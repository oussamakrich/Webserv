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
            background-color: #f4f4f4;
            margin: 20px;
            text-align: center;
        }

        h2 {
            color: #333;
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
    <li><a href="Delete.php"> Post </a>    </li>
    <li><a href="Python-Cgi.php" > Python Cgi </a>  </li>
    <li><a href="Session.php"> Session</a>  </li>
    <li><a href="phpCgi.php"> php Cgi  </a> </li>
    <li><a href="logout.php" class = "logout">Log out </a></li>
</ul>
</nav>
<body>
	<h2>Upload Image</h2>

    <label for="imageInput">Select Image</label>
    <input type="file" id="imageInput" accept="image/*" />
    <button onclick="uploadImage()">Upload</button>
    <div id="result"></div>

    <h2>Upload PDF</h2>
    <label for="pdfInput">Select PDF</label>
    <input type="file" id="pdfInput" accept=".pdf" />
    <button onclick="uploadPDF()">Upload</button>
    <div id="result"></div>

    <h2>Upload Video</h2>
    <label for="videoInput">Select Video</label>
    <input type="file" id="videoInput" accept="video/*" />
    <button onclick="uploadVideo()">Upload</button>
    <div id="result"></div>

	<h2>Upload Multiple Images</h2>
<div id="dropArea" ondrop="handleDrop(event)" ondragover="handleDragOver(event)" ondragenter="handleDragEnter(event)" ondragleave="handleDragLeave(event)">
    <p>Drag and drop images here</p>
</div>
<button onclick="uploadImages()">Upload</button>
<div id="result"></div>

<script>
const uploadImages = async () => {
    const input = document.getElementById("imageInput");
    const files = input.files;

    if (files.length > 0) {
        const formData = new FormData();
        for (let i = 0; i < files.length; i++) {
            formData.append("images[]", files[i]);
        }

        try {
            const response = await fetch("/upload", {
                method: "POST",
                body: formData
            });

            const result = await response.text();
            document.getElementById("result").innerHTML = result;
        } catch (error) {
            console.error("Error:", error);
        }
    } else {
        alert("Please select one or more image files.");
    }
};

const handleDrop = (event) => {
    event.preventDefault();
    document.getElementById("dropArea").classList.remove("dragover");
    const files = event.dataTransfer.files;
    document.getElementById("imageInput").files = files;
};

const handleDragOver = (event) => {
    event.preventDefault();
};

const handleDragEnter = (event) => {
    event.preventDefault();
    document.getElementById("dropArea").classList.add("dragover");
};

const handleDragLeave = (event) => {
    event.preventDefault();
    document.getElementById("dropArea").classList.remove("dragover");
};
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
      function uploadPDF() {
        var input = document.getElementById("pdfInput");
        var file = input.files[0];

        if (file) {
          var formData = new FormData();
          formData.append("pdf", file);

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
          alert("Please select a PDF file.");
        }
      }
      function uploadVideo() {
        var input = document.getElementById("videoInput");
        var file = input.files[0];

        if (file) {
          var formData = new FormData();
          formData.append("video", file);

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
          alert("Please select a video file.");
        }
      }
	  function uploadImages() {
    var input = document.getElementById("imageInput");
    var files = input.files;

    if (files.length > 0) {
        var formData = new FormData();
        for (var i = 0; i < files.length; i++) {
            formData.append("images[]", files[i]);
        }

        fetch("/upload", {
            method: "POST",
            body: formData
        })
        .then(response => response.text())
        .then(result => {
            document.getElementById("result").innerHTML = result;
        })
        .catch(error => {
            console.error("Error:", error);
        });
    } else {
        alert("Please select one or more image files.");
    }
}
</script>

</body>
<footer>
    Http Hustler Web Server &copy 2023 - 1337
 </footer>


</html>
