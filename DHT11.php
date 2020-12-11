<html>
<body>

<?php

$dbname = 'air_quality';
$dbuser = 'root';  
$dbpass = ''; 
$dbhost = 'localhost'; 

$connect = @mysqli_connect($dbhost,$dbuser,$dbpass,$dbname);

if(!$connect){
	echo "Error: " . mysqli_connect_error();
	exit();
}

echo "Connection Success!<br><br>";

$temperature = $_GET["temperature"];
$humidity = $_GET["humidity"];
$raw_measurement = $_GET["raw_measurement"];
$dust_density = $_GET["dust_density"];


$query = "INSERT INTO sensors_data (temperature, humidity, raw_measurement, dust_density) VALUES ('$temperature', '$humidity', '$raw_measurement', '$dust_density')";
$result = mysqli_query($connect,$query);

echo "Insertion Success!<br>";

?>
</body>
</html>