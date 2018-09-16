<html>
<body>
<?php
date_default_timezone_set("Asia/Bangkok");
$con = mysqli_connect('localhost', 'khanhpham', '123456', 'smart_class');

if (!$con) {
    die('Could not connect: ' . mysqli_error($con));
}

if (isset($_GET['MSSV'], $_GET['UID'], $_GET['HoTen'])) {
    $now   = date("Y-m-d h:i:s");
    $MSSV  = $_GET['MSSV'];
    $UID   = $_GET['UID'];
    $HoTen = $_GET['HoTen'];
    
    $query1 = "insert into DanhSachSinhVien values('$MSSV','$UID','$HoTen','$now')";
    $query2 = "insert into DiemDanh values('$MSSV','0','0','0','0','0','0','0','0')";
    
    if (mysqli_query($con, $query1) && mysqli_query($con, $query2)) {
        header('location:dssv_edit.php');
    }
}

?>
</body>
</html>