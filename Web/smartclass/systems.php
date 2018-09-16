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
	// Print the message and exit the current script
    die("Connection failed: " . $conn->connect_error);
}

$sql    = "SELECT MSSV FROM DanhSachSinhVien WHERE UID='$uid'";
$result = $conn->query($sql);
$row    = mysqli_fetch_array($result);

if ($result->num_rows > 0) {
    $thu        = date("l");
    $MSSV       = $row['MSSV'];
    $sql_update = "update DiemDanh set DiemDanh.$thu = '1' where MSSV='$MSSV'";
    
    $sql_check    = "SELECT * from DiemDanh where MSSV='$MSSV'";
    $result_check = $conn->query($sql_check);
    $row_check    = mysqli_fetch_array($result_check);
    $check        = $row_check[$thu];
    
    $sql_reset = "update DiemDanh set Monday='0',Tuesday='0',Wednesday='0',Thursday='0',Friday='0',Saturday='0',Sunday='0' where MSSV='$MSSV'";
    
    if (date("l") == "Monday") {
        if ($conn->query($sql_reset) === TRUE);
    }
    
    if ($check == 0) {
        
        if ($conn->query($sql_update) === TRUE) {
            echo "Updated";
            $sql_cong = "update DiemDanh set DiemDanh.Days = Days + 1";
            if ($conn->query($sql_cong) === TRUE);
        } else {
            echo "Error";
        }
    }
    
    else {
        echo "Updated";
    }
} else {
    echo "Error";
}

$conn->close();

?>	