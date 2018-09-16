<?php
date_default_timezone_set("Asia/Bangkok");
$time_now   = date("Y-m-d h:i:s");
$servername = "localhost";
$username   = "khanhpham";
$password   = "123456";
$dbname     = "smart_class";
$uid        = $_GET['uidPC'];

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql    = "SELECT * FROM ThongBao";
$result = $conn->query($sql);
if ($result->num_rows > 0) {
    while ($row = $result->fetch_assoc()) {
        echo $row["NoiDung"] . "                              ";
    }
} else {
    echo "Error";
}

$conn->close();

?>