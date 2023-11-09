<?php
header("Content-Type : text/html");
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
    <li><a href=" index.php">  Home </a>    </li>
    <li><a href=" Upload.php"> Upload</a>  </li>
    <li><a href="Download.php"  class = "select_menu"> Download </a>   </li>
    <li><a href="Get.php">  Get </a>     </li>
    <li><a href="Delete.php"> Post </a>    </li>
    <li><a href="Post.php    "> Delete </a>  </li>
    <li><a href="Session.php"> Session</a>  </li>
    <li><a href="Python-Cgi.php" > Python Cgi </a>  </li>
    <li><a href="phpCgi.php"> php Cgi  </a> </li> 
    <li><a href="logout.php" class = "logout">Log out </a></li>
</ul>
</nav>

<?php
$dirPath = "/Users/ebennamr/Desktop/WebServer";
echo " <h1> ---Available  File --</h1>";
echo "<br><br>";
$files = scandir($dirPath);  
foreach ($files as $file) {
    $filePath = $dirPath . '/' . $file;
    if (is_file($filePath)) {
        echo "<div class='download'>  <a  href= \"./";
        echo   $file;
        echo      "\">" . $file . "</a> </div>";
     

    }
}

?>

<footer>  
    Http Hustler Web Server &copy 2023 - 1337 
 </footer>  

</html>
