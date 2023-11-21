#!/usr/bin/env python3
import cgi

print("Content-type: text/html\n")

form = cgi.FieldStorage()

html = """
<!DOCTYPE html>

<html>

<head>
    <link rel="stylesheet" href="style.css"/>
</head>
 <h1> Hustler Server </h1>
<nav  class="navbar">
            <ul>
            <li><a href="index.html">  Home </a>    </li>
            <li><a href=" Upload.html"> Upload</a>  </li>
            <li><a href="Session.php"> Session</a>  </li>
            <li><a href="Python-Cgi.py" class = "select_menu"> Python Cgi </a>  </li>
            <li><a href="phpCgi.php"> php Cgi  </a> </li> 
    </ul>
</nav>
<body>
    <h1>Personalized Greeting</h1>
    <form method="post" action="">
        <label for="name">Enter your name:</label>
        <input type="text" name="name" required><br>

        <label for="age">Enter your age:</label>
        <input type="text" name="age" required><br>

        <input type="submit" value="Generate Greeting">
    </form>
"""

if "name" in form and "age" in form:
    name = form["name"].value
    age = form["age"].value

    # Print personalized greeting
    html += f"<p>Hello, {name}! You are {age} years old.</p>"

html += """
</body>
</html>
"""

print(html)
