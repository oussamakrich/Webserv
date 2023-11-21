<!DOCTYPE html>

<html>

<head>
    <link rel="stylesheet" href="style.css"/>
</head>
 <h1> Hustler Server </h1>
 <nav  class="navbar">
    <ul>
    <li><a href="index.html">  Home </a>    </li>
    <li><a href="Upload.html"> Upload</a>  </li>
    <li><a href="Session.php"> Session</a>  </li>
    <li><a href="Python-Cgi.py" > Python Cgi </a>  </li>
    <li><a href="phpCgi.php" class = "select_menu"> php Cgi  </a> </li> 
</ul>
</nav>

<h1> PHP Password Generetor  </h1>
<?php 
    $chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%&*_";
    $pass = substr( str_shuffle( $chars ), 0, 8 );
    echo "PassWord : $pass";
?> 
        <form method="GET" action="phpCgi.php" >     	
                <td align="center"><br><input type="submit" value="Generete Password">
        </form>

<footer>  
    Http Hustler Web Server &copy 2023 - 1337 
 </footer>  
 
</html>
