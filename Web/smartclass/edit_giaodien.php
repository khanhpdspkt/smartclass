<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css">
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
<script src="https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js"></script>
<div class="container">
<img class="img-responsive" src="/img/skpt_banner.jpg" alt="spkt_banner" width="590" height="135">
</div>
</head>

<body>

<?php



$con = mysqli_connect('localhost','khanpham','123456','smart_class');
if (!$con) {
    die('Could not connect: ' . mysqli_error($con));
}

mysqli_select_db($con,"ajax_demo");

$MSSV=$_GET['MSSV'];

$sql="SELECT * FROM DanhSachSinhVien where MSSV='$MSSV'";
$result = mysqli_query($con,$sql);

echo "<div class='container'>
<table class='table table-striped'>
<tr>
<th>MSSV</th>
<th>UID</th>
<th>Ho Ten</th>
<th>Thoi Gian</th>
</tr>";
while($row = mysqli_fetch_array($result)) {
    echo "<tr>";
    echo "<td>" . $row['MSSV'] . "</td>";
    echo "<td>" . $row['UID'] . "</td>";
    echo "<td>" . $row['HoTen'] . "</td>";
    echo "<td>" . $row['ThoiGian'] . "</td>";
    echo "</tr>";
}
echo "<tr><form action='edit.php' method='get'><td><input type='text' name='MSSV' value='$MSSV'></td>";
echo "<td><input type='text' name='UID'></td>";
echo "<td><input type='text' name='HoTen'></td>";
echo "<td><input type='submit' value='Edit'></td></form></tr>";
echo "</table></div><br>";

mysqli_close($con);
?>

</body>
</html>