<!DOCTYPE html>
<!--
This is a starter template page. Use this page to start your new project from
scratch. This page gets rid of all links and provides the needed markup only.
-->
<html>
<head>
  <meta charset="utf-8">
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <title>Smart Class</title>
  <!-- Tell the browser to be responsive to screen width -->
  <meta content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" name="viewport">
  <!-- Bootstrap 3.3.6 -->
  <link rel="stylesheet" href="bootstrap/css/bootstrap.min.css">
  <!-- Font Awesome -->
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.5.0/css/font-awesome.min.css">
  <!-- Ionicons -->
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/ionicons/2.0.1/css/ionicons.min.css">
  <!-- Theme style -->
  <link rel="stylesheet" href="dist/css/AdminLTE.min.css">
  <!-- AdminLTE Skins. We have chosen the skin-blue for this starter
        page. However, you can choose any other skin. Make sure you
        apply the skin class to the body tag so the changes take effect.
  -->
  <link rel="stylesheet" href="dist/css/skins/skin-blue.min.css">

  <!-- HTML5 Shim and Respond.js IE8 support of HTML5 elements and media queries -->
  <!-- WARNING: Respond.js doesn't work if you view the page via file:// -->
  <!--[if lt IE 9]>
  <script src="https://oss.maxcdn.com/html5shiv/3.7.3/html5shiv.min.js"></script>
  <script src="https://oss.maxcdn.com/respond/1.4.2/respond.min.js"></script>
  <![endif]-->
</head>
<!--
BODY TAG OPTIONS:
=================
Apply one or more of the following classes to get the
desired effect
|---------------------------------------------------------|
| SKINS         | skin-blue                               |
|               | skin-black                              |
|               | skin-purple                             |
|               | skin-yellow                             |
|               | skin-red                                |
|               | skin-green                              |
|---------------------------------------------------------|
|LAYOUT OPTIONS | fixed                                   |
|               | layout-boxed                            |
|               | layout-top-nav                          |
|               | sidebar-collapse                        |
|               | sidebar-mini                            |
|---------------------------------------------------------|
-->
<body class="hold-transition skin-blue sidebar-mini">
<div class="wrapper">

  <!-- Main Header -->
  <header class="main-header">

    <!-- Logo -->
    <a href="index.html" class="logo">
      <!-- mini logo for sidebar mini 50x50 pixels -->
      <span class="logo-mini"><b>S</b>C</span>
      <!-- logo for regular state and mobile devices -->
      <span class="logo-lg"><b>Smart</b>Class</span>
    </a>

    <!-- Header Navbar -->
    <nav class="navbar navbar-static-top" role="navigation">
      <!-- Sidebar toggle button-->
      <a href="#" class="sidebar-toggle" data-toggle="offcanvas" role="button">
        <span class="sr-only">Toggle navigation</span>
      </a>
      <!-- Navbar Right Menu -->
      <div class="navbar-custom-menu">
        <ul class="nav navbar-nav">         
          
          
        </ul>
      </div>
    </nav>
  </header>
  <!-- Left side column. contains the logo and sidebar -->
  <aside class="main-sidebar">

    <!-- sidebar: style can be found in sidebar.less -->
    <section class="sidebar">

      
      <!-- Sidebar Menu -->
      <ul class="sidebar-menu">
        <li class="header">Chức năng chính</li>
        <!-- Optionally, you can add icons to the links -->
        <li class="active"><a href="#"><i class="fa fa-th"></i> <span>Home</span></a></li>
        <li><a href="dssv.php"><i class="fa fa-table"></i> <span>Danh sách sinh viên</span></a></li>   
        <li><a href="dssv_edit.php"><i class="fa fa-edit"></i> <span>Thay đổi danh sách</span></a></li>
        <li><a href="dssv_edit.php"><i class="fa fa-lightbulb-o"></i> <span>Điều Khiển thiết bị</span></a></li>
        <li><a href="dssv_edit.php"><i class="fa fa-download"></i> <span>FOTA</span></a></li> 		
      </ul>
      <!-- /.sidebar-menu -->
    </section>
    <!-- /.sidebar -->
  </aside>

  <!-- Content Wrapper. Contains page content -->
  <div class="content-wrapper">
    <!-- Content Header (Page header) -->
    <section class="content-header">
      <h1>
        Điều khiển và giám sát thiết bị trong phòng
      </h1>
      <ol class="breadcrumb">
        <li><a href="index.html"><i class="fa fa-dashboard"></i> Home</a></li>
        <li class="active"><a href="#">Điều khiển và giám sát thiết bị trong phòng</a></li>
      </ol>
    </section>
	
    <!-- Main content -->
    <section class="content">
		<div class="row">
			<div class="col-xs-12">
			<div class="box">
				<div class="box-header">
				  <h3 class="box-title">Điều khiển và giám sát thiết bị trong phòng</h3>
				</div>
				<!-- /.box-header -->
					<div class="box-body">
					  <table id="example2" class="table table-bordered table-hover">
						<thead>
						<tr>
							<th>MSSV</th>
							<th>UID</th>
							<th>Ho Ten</th>
							<th>Thoi Gian</th>
						</tr>
						</thead>
						<tbody>
							<?php
								$con = mysqli_connect('localhost','khanhpham','123456','smart_class');
								if (!$con) {
									die('Could not connect: ' . mysqli_error($con));
								}
								
								$sql="SELECT * FROM DanhSachSinhVien";
								$result = mysqli_query($con,$sql);
								
								while($row = mysqli_fetch_array($result)) {
									echo "<tr>";
									echo "<td>" . $row['MSSV'] . "</td>";
									echo "<td>" . $row['UID'] . "</td>";
									echo "<td>" . $row['HoTen'] . "</td>";
									echo "<td>" . $row['ThoiGian'] . "</td>";
									
									echo "<td><a href='dssv_edit_giaodien.php?MSSV=". $row['MSSV'] ."'>Edit</a></td>";
									echo "<td><a href='delete.php?MSSV=". $row['MSSV'] ."'>Delete</a></td>";
									
									echo "</tr>";
								}
								
								echo "<tr><form action='add.php' method='get'><td><input type='text' name='MSSV'></td>";
								echo "<td><input type='text' name='UID'></td>";
								echo "<td><input type='text' name='HoTen'></td>";
								echo "<td><input type='submit' value='Add'></td></form></tr>";
								mysqli_close($con);
							?>
						</tbody>
					  </table>
					</div>
					<!-- /.box-body -->
			</div>
			<!-- /.box -->
			 
					 
			 
			</div>
      </div>
	</section>
    <!-- /.content -->
  </div>
  <!-- /.content-wrapper -->

  <!-- Main Footer -->
  <footer class="main-footer">
    <!-- To the right -->
    <div class="pull-right hidden-xs">
      Pham Duy Khanh
    </div>
    <!-- Default to the left -->
    <strong>Final project: <a href="#">Smart Class</a></strong>
  </footer>

</div>
<!-- ./wrapper -->

<!-- REQUIRED JS SCRIPTS -->

<!-- jQuery 2.2.3 -->
<script src="plugins/jQuery/jquery-2.2.3.min.js"></script>
<!-- Bootstrap 3.3.6 -->
<script src="bootstrap/js/bootstrap.min.js"></script>
<!-- AdminLTE App -->
<script src="dist/js/app.min.js"></script>

<!-- Optionally, you can add Slimscroll and FastClick plugins.
     Both of these plugins are recommended to enhance the
     user experience. Slimscroll is required when using the
     fixed layout. -->
</body>
</html>
