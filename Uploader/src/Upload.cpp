#include "../include/Upload.hpp"
#include <fstream>
#include <cstdio>
#include <netdb.h>
#include <ostream>
#include <string>
#include <sys/_types/_size_t.h>

Upload::Upload(Server &ser, Response &res):ser(ser), res(res)
{
	Uploader();
}

/*	This function will extract the boundary and the path of the file from the request
	 * 	and will save them in the response object
*/

bool Upload::_check_file_open_in(std::ifstream &file)
{
	if (!file.is_open())
	{
		std::cout << YELLOW"Warning:"<<  RESET" unable to upload the file (2)\n";
		res.setCode(500);
		res.stillSend = false;
		res.iHaveUpload = false;
		return false;
	}
	return true;
}
bool Upload::_check_file_open_out(std::ofstream &file)
{
	if (!file.is_open())
	{
		std::cout << YELLOW"Warning:"<<  RESET" unable to upload the file (3)\n";
		res.setCode(507);
		res.stillSend = false;
		res.iHaveUpload = false;
		return false;
	}
	return true;
}

void Upload::multipart()
{
	char	*buffer;
	std::ifstream file(res._source_file.c_str(), std::ios::binary);
	if (!Upload::_check_file_open_in(file))
		return;
	file.seekg(res._seek_pos);	//seek to the last position
	buffer = new char[BOUNDARY_READ_SIZE];

	file.read(buffer, BOUNDARY_READ_SIZE);

	res.byte = res.byte + Byte(buffer, file.gcount());

	delete [] buffer;

	_pos_of_boundary = res.byte.find(res._boundary.c_str(), res._boundary.size());
	std::string _end = res._boundary + "--";
	_pos_of_end_boundary = res.byte.find((_end).c_str(), _end.size());
	if (_pos_of_end_boundary == 0)
	{
		res.setCode(201);
				std::cout << GREEN"Succes:"<<  RESET" File Uploaded successfully " << res.getCode() << "\n";
		file.close();
		res.stillSend = false;
		res.iHaveUpload = false;
		return;
	}

	switch (_pos_of_boundary)
	{
		case 0:
			if (!Upload::_extract_headers())								return(res.byte.clear());// this will fill the file name
			_pos_of_boundary = res.byte.find(res._boundary.c_str(), res._boundary.size());
			_pos_of_end_boundary = res.byte.find((res._boundary + "--").c_str(), res._boundary.size() + 2);
			if (_pos_of_end_boundary == 0)
			{
				res.setCode(201);
				std::cout << GREEN"Succes:"<<  RESET" File Uploaded successfully " << res.getCode() << "\n";

				file.close();
				res.stillSend = false;
				res.iHaveUpload = false;
				res.byte.clear();
				return;
			}
			{
				std::ofstream _out_file(res.location->getUploadPath() + "/" + res._file_uploading, std::ios::binary | std::ios::app);
				if (!Upload::_check_file_open_out(_out_file))			return(res.byte.clear());
				char *extra = new char[res._boundary.size()];
				file.read(extra, res._boundary.size());

				if (_pos_of_boundary == -1)
				{
					_out_file.write(res.byte.getData(), res.byte.size());
					res._seek_pos += res.byte.size();
					res.byte.clear();
					res.stillSend = true;
					res.iHaveUpload = true;
				}
				else
				{
					_out_file.write(res.byte.getData(), _pos_of_boundary);
					_out_file.close();
					res._seek_pos += _pos_of_boundary;
					res.byte.clear();
					res.stillSend = true;
					res.iHaveUpload = true;
				}
			}
			break;
		case -1:
			{
				std::ofstream _out_file(res.location->getUploadPath() + "/" + res._file_uploading, std::ios::binary | std::ios::app);
				if (!Upload::_check_file_open_out(_out_file))			return(res.byte.clear());
				_out_file.write(res.byte.getData(), res.byte.size());
				_out_file.close();
				res._seek_pos += res.byte.size();
				res.byte.clear();
				res.stillSend = true;
				res.iHaveUpload = true;
			}
			break;
		default :
			{
				std::ofstream _out_file(res.location->getUploadPath() + "/" + res._file_uploading, std::ios::binary | std::ios::app);
				if (!Upload::_check_file_open_out(_out_file))			return(res.byte.clear());
				_out_file.write(res.byte.getData(), _pos_of_boundary);
				_out_file.close();
				res._seek_pos += _pos_of_boundary;
				res.byte.clear();
				res.stillSend = true;
				res.iHaveUpload = true;
			}
			break;
	}
}

bool Upload::_extract_headers()
{
	int	increment;

	increment = 4;
	long long pos = res.byte.find("\r\n\r\n", 4);
	long long pos2 = res.byte.find("\n\n", 2);
	if (pos == -1 && pos2 == -1)
	{
		std::cout << YELLOW"Warning:"<<  RESET" unable to upload the file (4)\n";
		res.setCode(507);
		res.stillSend = false;
		res.iHaveUpload = false;
		return false;
	}
	if (pos == -1 || (pos2 != -1 && pos2 < pos))
	{
		increment = 2;
		pos = pos2;
	}
	_headers = nullTerminate(res.byte.substr(0, pos), pos);
	res._file_uploading = _headers.substr(_headers.find("filename=") + 9);
	if (res._file_uploading[0] == '"')
		res._file_uploading = res._file_uploading.substr(1);
	res._file_uploading = res._file_uploading.substr(0, res._file_uploading.find('"'));
	res._seek_pos += pos + increment;
	res.byte.resize(pos + increment);
	return true;
}

void Upload::Uploader()
{
	std::string sourceFile = res._source_file;
	std::string destinationFile = res.location->getUploadPath() + "/" + res._single_file_name;

	if (res.getUploadStat() || res._is_multipart_form)
	{
		multipart();
	}
	else
	{
		if (rename(sourceFile.c_str(), destinationFile.c_str()) == 0)
		{
			std::cout<< YELLOW"File Uploaded successfully: " <<U_WHITE<< destinationFile << "\n";
			res.setCode(201);
			res.stillSend = false;
			res.iHaveUpload = false;
		}
		else
		{
			std::cout << YELLOW"Warning:"<<  RESET" unable to upload the file (1)\n";
			res.setCode(500);
			res.stillSend = false;
			res.iHaveUpload = false;
		}
	}

}

Upload::~Upload(){}
