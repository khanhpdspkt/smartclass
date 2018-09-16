<html>
<body>
<?php
$con = mysqli_connect('localhost','khanhpham','123456','smart_class');
if (!$con) {
    die('Could not connect: ' . mysqli_error($con));
}
if(isset($_GET['MSSV']))
{
    $MSSV=$_GET['MSSV'];
    $query1="delete from DanhSachSinhVien where MSSV='$MSSV'";
    $query2="delete from DiemDanh where MSSV='$MSSV'";
    if(mysqli_query($con,$query1) && mysqli_query($con,$query2))
    {
        header('location:dssv_edit.php');
    }
}
?>
</body>
</html>