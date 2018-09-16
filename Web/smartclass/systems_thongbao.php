<?php
			$servername = "localhost";
                        $username = "id115264_u814508077_user";
                        $password = "truongchidung";
                        $dbname = "id115264_u814508077_rfid";
                        $thongbao = $_GET['thongbao'];

                        // Create connection
                        $conn = new mysqli($servername, $username, $password, $dbname);
                        // Check connection
                        if ($conn->connect_error) {
                            die("Connection failed: " . $conn->connect_error);
                        }

                        $sql_update = "UPDATE ThongBao SET NoiDung='$thongbao'";
                            if ($conn->query($sql_update) === TRUE) {
				echo "Updated";
                            }
                            else {
				echo "Error";
                            }
                            
                        $conn->close();

?>