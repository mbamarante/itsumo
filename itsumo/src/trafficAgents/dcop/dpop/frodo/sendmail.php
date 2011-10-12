<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
  <title>Send an email to the participants</title>
  <link rel="stylesheet" type="text/css" href="/lia.css">
</head>
<body>

<?php

include ("/var/www/html/Internal/Sysadmin/safe/authenticate.php");

#global $database;
#$registerTable="csclp04_reg";



$name=$_POST["name"];
$email=$_POST["email"];
$institution=$_POST["institution"];
$interest=$_POST["interest"];

$ipaddr=$_SERVER['REMOTE_ADDR'];
logmessage("/var/log/sync_www/web_frodo_sendmail.log", $ipaddr."\t".$email."\t".date(r)."\t".$name."\t".$institution."\t".$interest."\n");


#echo "Request from ".$ipaddr." for user ".$email." on ".date(r);


    			echo "<br /> \n";
			$newmsg="The URL for downloading FRODO is this:" .
				"http://liawww.epfl.ch/frodo/apetcu-frodo_641.tgz\r\n" . 
				"Make sure to visit http://liawww.epfl.ch/frodo/ often, since there will be updated releases from time to time.\r\n" . 
				"Thank you for your interest, and I am awaiting your valuable feedback with most interest.\r\n".
				"Adrian Petcu";
			echo $newmsg;
			echo "<h1>Thank you!</h1>".
			    "<br />Message sent to ".$email;
    			echo "\n</p>\n";
                	mail($email, "[FRODO] - download URL for ".$email, $newmsg, "From: Adrian Petcu <adrian.petcu@epfl.ch>\r\n" .
                                    "X-Mailer: PHP/" . phpversion());
echo '<hr>'.generatedMessage();

?>
</body>
</html>

