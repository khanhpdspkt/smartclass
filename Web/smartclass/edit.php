<html>
<body>
<?php
$con = mysqli_connect('localhost','khanhpham','123456','smart_class');
if (!$con) {
    die('Could not connect: ' . mysqli_error($con));
}

if(isset($_GET['MSSV'],$_GET['UID'],$_GET['HoTen']))
{
$MSSV=$_GET['MSSV'];
$UID=$_GET['UID'];
$HoTen=$_GET['HoTen'];

$query1="update DanhSachSinhVien set UID='$UID', HoTen='$HoTen' where MSSV='$MSSV'";

if(mysqli_query($con,$query1))
{
header('location:dssv_edit.php');
}
}

?>
</body>
</html>