<?php
session_start();

if (!(isset($_SESSION['user']) && isset($_SESSION['pass'])))
{
	header('location: login.php');
    exit;
}
?>


<!DOCTYPE html>

<html>

<head>
    <link rel="stylesheet" href="style.css"/>
    <style>
        body {
            font-family: Arial, sans-serif;
            /* background-color: #f4f4f4; */
            margin: 20px;
            text-align: center;
        }

        h2 {
            color: #fff;
        }

        input[type="file"] {
            display: none;
        }

        label, #dropArea {
            display: block;
            background-color: #4D4C7D;
            color: #fff;
            padding: 10px;
            text-align: center;
            cursor: pointer;
            border-radius: 5px;
            margin-bottom: 10px;
        }

        button {
            background-color: #2ecc71;
            color: #fff;
            padding: 10px;
            border: none;
            border-radius: 5px;
            cursor: pointer;
            margin: 0 auto;
        }

        #result {
            margin-top: 20px;
            font-weight: bold;
            color: #333;
        }

        #dropArea {
            border: 2px dashed #3498db;
            padding: 20px;
            transition: background-color 0.3s;
        }

        #dropArea.dragover {
            background-color: #ecf0f1;
        }
    </style>
</head>
 <h1> Hustler Server </h1>
 <nav  class="navbar">
    <ul>
    <li><a href="index.php">  Home </a>    </li>
    <li><a href="Upload.php" class = "select_menu"> Upload</a>  </li>
    <li><a href="Download.php"> Download </a>   </li>
    <li><a href="Get.php">  Get </a>     </li>
    <li><a href="Delete.php"> Delete </a>    </li>
    <li><a href="Post.php"> Post </a>    </li>
    <li><a href="Python-Cgi.php" > Python Cgi </a>  </li>
    <li><a href="Session.php"> Session</a>  </li>
    <li><a href="phpCgi.php"> php Cgi  </a> </li>
    <li><a href="logout.php" class = "logout">Log out </a></li>
</ul>
</nav>
<body>
<<<<<<< HEAD

    <h1>File Upload Example</h1>

    <form id="fileUploadForm" enctype="multipart/form-data">
        <label for="image">Upload Image:</label>
        <input type="file" id="image" name="image" accept="image/*"><br>

        <label for="pdf">Upload PDF:</label>
        <input type="file" id="pdf" name="pdf" accept=".pdf"><br>

        <label for="video">Upload Video:</label>
        <input type="file" id="video" name="video" accept="video/*"><br>

        <button type="button" onclick="uploadFiles()">Upload Files</button>
    </form>

    <script>
        function uploadFiles() {
            var formData = new FormData(document.getElementById('fileUploadForm'));

            // Use Fetch API to send the form data to the server
            fetch('/upload', {
                method: 'POST',
                body: formData,
            })
            .then(response => response.text())
            .then(data => {
                console.log(data);
                alert(data);
            })
            .catch(error => {
                console.error('Error:', error);
            });
        }
    </script>
=======
	  <h2>Upload Image</h2>
    <label for="imageInput">Select Image</label>
    <input type="file" id="imageInput" accept="image/*" />
    <button onclick="uploadImage()">Upload</button>
    <div id="result"></div>
<script>
function uploadImage() {
        var input = document.getElementById("imageInput");
        var file = input.files[0];

        if (file) {
          var formData = new FormData();
          formData.append("image", file);

          fetch("/upload", {
            method: "POST",
            body: formData,
          })
            .then((response) => response.text())
            .then((result) => {
              document.getElementById("result").innerHTML = result;
            })
            .catch((error) => {
              console.error("Error:", error);
            });
        } else {
          alert("Please select an image file.");
        }
      }
</script>
>>>>>>> 276bce64701b93fdb728c5ce3a24f390ac90be41

</body>
<footer>
    Http Hustler Web Server &copy 2023 - 1337
 </footer>


</html>
