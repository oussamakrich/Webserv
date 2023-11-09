<?php
session_start();

// $email = isset($_GET['user']) &&  $_GET['user'] == 	"a";
// $pass =  isset($_GET['pass']) &&  $_GET['pass'] == 	"a";

$email = isset($_POST['user']) &&  $_POST['user'] == 	"a";
$pass =  isset($_POST['pass']) &&  $_POST['pass'] == 	"a";
if ($email && $pass)
{
	$_SESSION['user'] = $_POST['user'];
	$_SESSION['pass'] = $_POST['pass'];
}
else if (isset($_SESSION['user']) && isset($_SESSION['pass']))
{

}
else
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
        <li><a href=" index.php" class = "select_menu">  Home </a>    </li>
        <li><a href=" Upload.php"> Upload</a>  </li>
        <li><a href="Download.php"> Download </a>   </li>
        <li><a href="Get.php">  Get </a>     </li>
        <li><a href="Delete.php"> Post </a>    </li>
        <li><a href="Post.php    "> Delete </a>  </li>
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

