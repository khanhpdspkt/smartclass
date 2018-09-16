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

$con = mysqli_connect('localhost','u814508077_user','truongchidung','u814508077_rfid');
if (!$con) {
    die('Could not connect: ' . mysqli_error($con));
}

mysqli_select_db($con,"ajax_demo");

$sql="SELECT * FROM DanhSachSinhVien";
$result = mysqli_query($con,$sql);

$sql_2="SELECT * FROM DiemDanh";
$result_2 = mysqli_query($con,$sql_2);

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
echo "</table></div><br>";


echo "<div class='container'>
<table class='table table-striped'>
<tr>
<th>MSSV</th>
<th>Monday</th>
<th>Tuesday</th>
<th>Wednesday</th>
<th>Thursday</th>
<th>Friday</th>
<th>Saturday</th>
<th>Sunday</th>
<th>Days</th>
</tr>";
while($row = mysqli_fetch_array($result_2)) {
    echo "<tr>";
    echo "<td>" . $row['MSSV'] . "</td>";

if( $row['Monday'] == 1) {
 echo "<td>Co mat</td>";}
else {
 echo "<td>Vang mat</td>";}

if( $row['Tuesday'] == 1) {
 echo "<td>Co mat</td>";}
else {
 echo "<td>Vang mat</td>";}

if( $row['Wednesday'] == 1) {
 echo "<td>Co mat</td>";}
else {
 echo "<td>Vang mat</td>";}

if( $row['Thursday'] == 1) {
 echo "<td>Co mat</td>";}
else {
 echo "<td>Vang mat</td>";}

if( $row['Friday'] == 1) {
 echo "<td>Co mat</td>";}
else {
 echo "<td>Vang mat</td>";}

if( $row['Saturday'] == 1) {
 echo "<td>Co mat</td>";}
else {
 echo "<td>Vang mat</td>";}

if( $row['Sunday'] == 1) {
 echo "<td>Co mat</td>";}
else {
 echo "<td>Vang mat</td>";}

echo "<td>" . $row['Days'] . "</td>";
    
    echo "</tr>";
}
echo "</table></div><br>";


mysqli_close($con);
?>
</body>
</html>																	