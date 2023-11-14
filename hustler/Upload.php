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

</body>
<footer>
    Http Hustler Web Server &copy 2023 - 1337
 </footer>


</html>
