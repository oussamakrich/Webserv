#!/bin/bash

# Set the file path and URL
file_path="/Users/moulmoud/Desktop/tt.png"
upload_url="http://10.13.2.8:8080/upload"

# if [[ $(uname) == "Linux" ]]; then
#     file_size=$(stat -c%s "$file_path")
# elif [[ $(uname) == "Darwin" ]]; then
#     file_size=$(stat -f%z "$file_path")
# else
#     echo "Unsupported operating system"
#     exit 1
# fi

# # Set headers and perform the upload
# curl -X POST -H "Content-Type: image/jpeg" -H "Content-Length: $file_size" --data-binary "@$file_path" "$upload_url"


curl -X POST -H "Transfer-Encoding: chunked" -H "Content-Type: image/jpeg" --data-binary "@$file_path" "$upload_url"
