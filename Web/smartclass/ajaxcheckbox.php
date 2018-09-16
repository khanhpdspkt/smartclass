<?php
	require "dbcon.php";
?>

<?php
		$stdv1 = $_POST["dv1"];
		$stdv2 = $_POST["dv2"];
		$stdv3 = $_POST["dv3"];
		$stdv4 = $_POST["dv4"];
		
		$sql1 = "UPDATE present SET Dv1 = $stdv1 ";
		mysqli_query($conn,$sql1);
		$sql2 = "UPDATE present SET Dv2 = $stdv2 ";
		mysqli_query($conn,$sql2);
		$sql3 = "UPDATE present SET Dv3 = $stdv3 ";
		mysqli_query($conn,$sql3);
		$sql4 = "UPDATE present SET Dv4 = $stdv4 ";
		mysqli_query($conn,$sql4);
		
?>