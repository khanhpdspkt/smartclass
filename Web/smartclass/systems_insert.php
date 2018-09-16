<?php
			date_default_timezone_set("Asia/Bangkok");
                        $time_now = date("Y-m-d h:i:s");                
                        $servername = "localhost";
                        $username = "id115264_u814508077_user";
                        $password = "truongchidung";
                        $dbname = "id115264_u814508077_rfid";
                        $uid = $_GET['uidPC'];

$MSSV = $_GET['mssv'];
$UID  = $_GET['uid'];
$HoTen = $_GET['hoten'];


                        // Create connection
                        $conn = new mysqli($servername, $username, $password, $dbname);
                        // Check connection
                        if ($conn->connect_error) {
                            die("Connection failed: " . $conn->connect_error);
                        } 


$sql = "INSERT INTO DanhSachSinhVien (MSSV, UID, HoTen, ThoiGian)
VALUES ('$MSSV', '$UID', '$HoTen', '2016-12-12 00:00:00')";

if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}
                        
                        $conn->close();

?>