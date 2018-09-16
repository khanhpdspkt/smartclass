<?php
  $con = mysqli_connect('localhost','u814508077_user','truongchidung','u814508077_rfid');
if (!$con) {
die('Could not connect: ' . mysqli_error($con));
}
  // Mảng JSON
  $response = array();
  $response["danhsach"] = array();
 //$response[""] = array();
  // Câu lệnh Select dùng để xem dữ liệu
$sql="SELECT * FROM DanhSachSinhVien";
$result = mysqli_query($con,$sql);
 
  //Đọc dữ liệu từ MySQL
  while($row = mysqli_fetch_array($result)){ 
    $t = array();
    $t["MSSV"] = $row["MSSV"];
    $t["UID"] = $row["UID"];
    $t["HoTen"] = $row["HoTen"];
    $t["ThoiGian"] = $row["ThoiGian"];
 
    // Mảng JSON
    array_push($response["danhsach"], $t);
 }
 
 // Thiết lập header là JSON
 header('Content-Type: application/json');
 
 // Hiển thị kết quả
 echo json_encode($response);
?>