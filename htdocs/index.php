<?php
if(isset($_GET['category']))
	$page_category_id = (int)$_GET['category'];
else
	$page_category_id = 0;

if(isset($_GET['index']))
	$page_index_id = (int)$_GET['index'];
else
	$page_index_id = 0;

$hinting_file_title = $page_category_id.'_'.$page_index_id.'.ucf';
if(!file_exists($hinting_file_title)) 
	$hinting_file_title = "0_0.ucf";

$hinting_file = fopen($hinting_file_title, "r");
$page_title = trim(fgets($hinting_file));
$date_1 = trim(fgets($hinting_file));
$date_2 = trim(fgets($hinting_file));

$element_count = 0;
while(!feof($hinting_file)) {
	$line = trim(fgets($hinting_file));
	$hint_types[$element_count] = substr($line, 0, 1);
	$hints[$element_count] = substr($line, 3);
	$element_count++;
}
fclose($hinting_file);
?>
<!DOCTYPE HTML>
<html>
<head>
	<title>David's Place - <?php echo $page_title; ?></title>
	<base href="http://87.98.216.46/">
	<style>
		a		{	color:#a5a5a5; }
		
		h6		{	margin-left: 25px; 
					margin-top: 5px; 
					margin-bottom: 10px; }
		
		h5		{	margin-left: 30px; 
					margin-bottom: 5px; }
		
		p		{	margin-left: 40px;
					margin-top: 5px; }
		
		img		{	margin-left: 60px; 
					margin-top: 5px;
					margin-bottom: 5px; }
	</style>
</head>

<body style="background-color:#262626;color:#a5a5a5;min-width:800px;">
<div style="float:left;width:540px;">
	<h4 style="margin-left:20px;margin-bottom:5px;"><?php echo $page_title; ?></h4>
	<h6><?php echo $date_1; ?></h6>
	<?php
	for($i=0;$i<$element_count;$i++) {
		if($hint_types[$i] == 'h') {
			echo "\n\t";
			echo '<h5>'.$hints[$i].'</h5>';
			echo "\n";
		} else if($hint_types[$i] == 'i') {
			$image_index = substr($hints[$i], 0, 3);
			$image_alt = substr($hints[$i], 4);
			echo "\n\t";
			echo '<a target="_blank" href="dvp_'.$image_index.'.png"><img src="dvp_'.$image_index.'s.jpg" alt="'.$image_alt.'"></a>';
			echo "\n";
		} else if($hint_types[$i] == 'p') {
			echo "\n\t";
			echo '<p>'.$hints[$i].'</p>';
			echo "\n";
		}
	}
	?>
	<h6><?php echo $date_2; ?></h6>
</div>

<div style="float:right;">
	<fieldset style="margin-right:60px;margin-top:40px;width:120px;">
		<legend>Navagation</legend>
		<a href="index.php?index=0&category=0">Home</a><br>
		<a href="index.php?index=1&category=0">All Pages</a><br>
		<a href="index.php?index=2&category=0">All Images</a><br>
		<a href="index.php?index=3&category=0">About sh.blog</a><br>
	</fieldset>
</div>
</body>
</html>
