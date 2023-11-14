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
    <li><a href=" index.php">  Home </a>    </li>
    <li><a href=" Upload.php"> Upload</a>  </li>
    <li><a href="Download.php"> Download </a>   </li>
    <li><a href="Get.php">  Get </a>     </li>
    <li><a href="Delete.php"> Post </a>    </li>
    <li><a href="Post.php" class = "select_menu"> Delete </a>  </li>
    <li><a href="Session.php"> Session</a>  </li>
    <li><a href="Python-Cgi.php"> Python Cgi </a>  </li>
    <li><a href="phpCgi.php"> php Cgi  </a> </li> 
    <li><a href="logout.php" class = "logout">Log out </a></li>
</ul>
</nav>


<footer>  
    Http Hustler Web Server &copy 2023 - 1337 
 </footer>  
 
</html>
