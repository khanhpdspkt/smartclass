<?php
    $con = mysqli_connect('localhost','khanhpham','123456','smart_class');
    if (!$con) {
        die('Could not connect: ' . mysqli_error($con));
    }
	$sql = "SELECT Temp, Humi FROM present";
	$result = mysqli_query($con, $sql);
	if(mysqli_num_rows($result) > 0)
			{
				$row = mysqli_fetch_assoc($result);				
			}
?>
<!doctype html>
<html>
<head>
<meta charset="utf-8">
<meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1">
<meta name="viewport" content="width=device-width, initial-scale=1">
		<!--<title>jQuery percircle.js Examples</title>-->
		<!--<script src="https://code.jquery.com/jquery-2.1.4.min.js"></script>-->
        <script src="jquery-3.2.0.min.js"></script>
        <script src="percircle-master/src/js/percircle.js"></script>
        <style type="text/css">
            body{
                background-color: #f5f5f5;
                margin: 0;
                padding: 0;
                font-family: Arial, "Helvetica Neue", Helvetica, sans-serif;
            }
            .page {
                margin: 150px auto;
                max-width: 960px;
            }
            h1{
                margin: 40px 0 60px 0;
            }
        </style>
        <link href="http://www.jqueryscript.net/css/jquerysctipttop.css" rel="stylesheet" type="text/css">
        <link rel="stylesheet" type="text/css" href="percircle-master/src/css/percircle.css">
    </head>

    <body>
    <?php //$temp = 28;
		  //$humi = 70;
	?>
    <div class="page" style="margin-top:5px; margin-left:5px; margin-bottom:2px">
            <!-- some colors -->
            <div class="clearfix" style="margin-top:1px; width:50%; height:45%; margin-left:5px; margin-bottom:2px">
                <div id="bluecircle" class="c100 p<?php echo $row["Temp"];?> big blue">
                    <span id="temp"><?php echo $row["Temp"];?>°C</span>
                    <div class="slice">
                        <div class="bar"></div>
                        <div class="fill"></div>
                    </div>
                </div>
				<div id="orangecircle" class="c100 p<?php echo $row["Humi"];?> orange">
                    <span id="humi"> <?php echo $row["Humi"];?>% </span>
                    <div class="slice">
                        <div class="bar"></div>
                        <div class="fill"></div>
                    </div>
                </div>
				
            </div>
            <!-- /some colors -->

        </div>
		<script src="percircle-master/src/js/custom.js"></script>
    </body>
</html>
