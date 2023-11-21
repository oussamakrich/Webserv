<?php
session_start();
?>


<html lang="en">
<head>
	<meta charset="UTF-8">
	<title> Hustler server </title>
	 <link rel="stylesheet" href="login.css"> 
</head>
<head>
    <link rel="stylesheet" href="style.css"/>
</head>
 <h1> Hustler Server </h1>
 <nav  class="navbar">
    <ul>
    <li><a href="index.html">  Home </a>    </li>
    <li><a href="Upload.html"> Upload</a>  </li>
    <li><a href="Session.php" class = "select_menu" > Session</a>  </li>
    <li><a href="Python-Cgi.py" > Python Cgi </a>  </li>
    <li><a href="phpCgi.php"> php Cgi  </a> </li> 
    <li><a href="logout.php" class = "logout">Log out </a></li>
</ul>
</nav>

<?php 
    if (isset($_POST['user']) && isset($_POST['pass']))
    {
        $_SESSION['user'] = $_POST['user'];
        $_SESSION['pass'] = $_POST['pass'];
    }

    if (isset($_SESSION['user']) && isset($_SESSION['pass']))
    {
        echo "<h1> php Sesseion  <h1>";
        echo "<h4> User : " . $_SESSION['user'] . "<h4>"; 
        echo "<h4> Password  : " . $_SESSION['pass'] . "<h4>"; 

    }
    else {
?>

<body>
        <form method="POST" action="Session.php" enctype="application/x-www-form-urlencode" >
        	<h1> Welcome to  Hustler server </h1>
        		<input type="text" name="user" placeholder="Enter User Name" value=""><br><br>       	
        	    <input type="password" name="pass" placeholder="Enter Password " value=""><br><br>        	
                <td align="center"><br><input type="submit" value="Log in ">
        </form>
<div>
</div>
<?php 
}
?>
<footer>  
    Http Hustler Web Server &copy 2023 - 1337 
 </footer>  
 
</html>
