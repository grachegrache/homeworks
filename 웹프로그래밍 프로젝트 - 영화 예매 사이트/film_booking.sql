-- MySQL dump 10.13  Distrib 5.7.9, for Win64 (x86_64)
--
-- Host: 127.0.0.1    Database: film_booking
-- ------------------------------------------------------
-- Server version	5.7.9-log

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `film`
--

DROP TABLE IF EXISTS `film`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `film` (
  `id_film` int(11) NOT NULL AUTO_INCREMENT,
  `name_film` varchar(45) NOT NULL,
  `age_phase` int(11) NOT NULL,
  `seat_num` int(11) NOT NULL,
  `watch_date` varchar(50) NOT NULL,
  PRIMARY KEY (`id_film`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `film`
--

LOCK TABLES `film` WRITE;
/*!40000 ALTER TABLE `film` DISABLE KEYS */;
INSERT INTO `film` VALUES (2,'star wars',18,9,'2015.11-2015.12'),(8,'martian',18,9,'2015.11-2015.12'),(9,'aboutTime',12,9,'2015.11-2015.12'),(11,'Damage',12,9,'2015.11-2015.12'),(12,'Hobbit',12,9,'2015.11-2015.12'),(13,'LesMiserables',12,9,'2015.11-2015.12'),(14,'Monster',15,9,'2015.11-2015.12'),(17,'Titanic',12,9,'2015.11-2015.12'),(20,'Blackswan',15,9,'2015.11.1-2015.12.2');
/*!40000 ALTER TABLE `film` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `film_reservation`
--

DROP TABLE IF EXISTS `film_reservation`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `film_reservation` (
  `id_mem` varchar(45) NOT NULL,
  `id_film` int(11) NOT NULL,
  `film_name` varchar(45) NOT NULL,
  `watch_date` varchar(45) NOT NULL,
  `resv_date` date NOT NULL,
  `seat_resv_no` int(11) NOT NULL,
  `resv_status` varchar(2) NOT NULL DEFAULT '-',
  PRIMARY KEY (`seat_resv_no`,`resv_date`,`id_film`,`id_mem`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `film_reservation`
--

LOCK TABLES `film_reservation` WRITE;
/*!40000 ALTER TABLE `film_reservation` DISABLE KEYS */;
/*!40000 ALTER TABLE `film_reservation` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `membership`
--

DROP TABLE IF EXISTS `membership`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `membership` (
  `id_mem` varchar(45) NOT NULL,
  `password` varchar(15) NOT NULL,
  `age_mem` int(11) NOT NULL,
  `rdate` date DEFAULT NULL,
  PRIMARY KEY (`id_mem`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `membership`
--

LOCK TABLES `membership` WRITE;
/*!40000 ALTER TABLE `membership` DISABLE KEYS */;
INSERT INTO `membership` VALUES ('aaa','aaa',18,'2015-12-03'),('bbb','bbb',18,'2015-12-03'),('ddd','ddd',12,'2015-12-06');
/*!40000 ALTER TABLE `membership` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `seat_no`
--

DROP TABLE IF EXISTS `seat_no`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!40101 SET character_set_client = utf8 */;
CREATE TABLE `seat_no` (
  `id_film` int(11) NOT NULL AUTO_INCREMENT,
  `seat1` varchar(45) DEFAULT NULL,
  `seat2` varchar(45) DEFAULT NULL,
  `seat3` varchar(45) DEFAULT NULL,
  `seat4` varchar(45) DEFAULT NULL,
  `seat5` varchar(45) DEFAULT NULL,
  `seat6` varchar(45) DEFAULT NULL,
  `seat7` varchar(45) DEFAULT NULL,
  `seat8` varchar(45) DEFAULT NULL,
  `seat9` varchar(45) DEFAULT NULL,
  PRIMARY KEY (`id_film`)
) ENGINE=InnoDB AUTO_INCREMENT=21 DEFAULT CHARSET=utf8;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `seat_no`
--

LOCK TABLES `seat_no` WRITE;
/*!40000 ALTER TABLE `seat_no` DISABLE KEYS */;
INSERT INTO `seat_no` VALUES (2,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(8,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(9,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(11,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(12,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(13,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(14,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(17,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL),(20,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
/*!40000 ALTER TABLE `seat_no` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Dumping routines for database 'film_booking'
--
/*!50003 DROP FUNCTION IF EXISTS `get_id_film` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `get_id_film`() RETURNS int(11)
begin

declare res int;
select max(id_film) into res from film;
set res=res+1;

return res;

end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!50003 DROP FUNCTION IF EXISTS `is_valid_date` */;
/*!50003 SET @saved_cs_client      = @@character_set_client */ ;
/*!50003 SET @saved_cs_results     = @@character_set_results */ ;
/*!50003 SET @saved_col_connection = @@collation_connection */ ;
/*!50003 SET character_set_client  = utf8 */ ;
/*!50003 SET character_set_results = utf8 */ ;
/*!50003 SET collation_connection  = utf8_general_ci */ ;
/*!50003 SET @saved_sql_mode       = @@sql_mode */ ;
/*!50003 SET sql_mode              = 'STRICT_TRANS_TABLES,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION' */ ;
DELIMITER ;;
CREATE DEFINER=`root`@`localhost` FUNCTION `is_valid_date`(watch_date varchar(45), resv_date date) RETURNS tinyint(1)
begin
declare start_date varchar(45);
declare end_date varchar(45);

set start_date=substring_index(watch_date, '-', 1);
set end_date=substring_index(watch_date, '-', -1);

if(start_date not like '%.%.%') then
	set start_date=concat(start_date, '.00');
end if;
if(end_date not like '%.%.%') then
	set end_date=concat(end_date, '.00');
end if;

if(resv_date between str_to_date(start_date, '%Y.%m.%d') and str_to_date(end_date, '%Y.%m.%d')) then
	return true;
else
	return false;
end if;

end ;;
DELIMITER ;
/*!50003 SET sql_mode              = @saved_sql_mode */ ;
/*!50003 SET character_set_client  = @saved_cs_client */ ;
/*!50003 SET character_set_results = @saved_cs_results */ ;
/*!50003 SET collation_connection  = @saved_col_connection */ ;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2015-12-06  3:51:45
