<!DOCTYPE html>
<html lang="en">

<head>
    <link rel="stylesheet" href="style.css" />
</head>

<body>

    <h1> Hustler Server </h1>
    <nav class="navbar">
        <ul>
            <li><a href="index.html"> Home </a></li>
            <li><a href="Upload.html"> Upload </a></li>
            <li><a href="Download.html"> Download </a></li>
            <li><a href="Get.html"> Get </a></li>
            <li><a href="Delete.php" class="select_menu"> Delete </a></li>
            <li><a href="Post.html"> Post </a></li>
            <li><a href="Session.php"> Session </a></li>
            <li><a href="Python-Cgi.py"> Python Cgi </a></li>
            <li><a href="phpCgi.php"> php Cgi </a></li>
        </ul>
    </nav>

    <h1>File Directory</h1>

    <ul id="fileList">
        <?php
        $directoryPath = '/Users/okrich/Desktop/project/webserv/Delete/';

        $files = scandir($directoryPath);
        foreach ($files as $file) {
            if ($file != '.' && $file != '..') {
                echo '<li>';
                echo '<span>' . $file . '</span>';
                echo '<button onclick="deleteFile(\'' . $file . '\')">Delete</button>';
                echo '</li>';
            }
        }
        ?>
    </ul>
    
    <footer>
        Http Hustler Web Server &copy 2023 - 1337
    </footer>

    <script>
        function deleteFile(fileName) {
            if (confirm('Are you sure you want to delete ' + fileName + '?')) {
                fetch('/Delete/' + fileName, {
                        method: 'DELETE',
                    })
                    .then(response => {
                        if (!response.ok)
                            console.error('Failed to delete file:', response.status);
                        location.reload();
                    })
                    .catch(error => {
                        console.error('Error:', error);
                    });
            }
        }
    </script>

</body>

</html>

