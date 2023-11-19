<?php

if ($_SERVER['REQUEST_METHOD'] === 'GET') 
{
    echo "<form method='POST'>";
    echo "<input name='id' />";
    echo "<input type='submit' value='set id' />";
    echo "</form>";

} elseif ($_SERVER['REQUEST_METHOD'] === 'POST') {
   
    echo "id = " . $_POST['id'];

}