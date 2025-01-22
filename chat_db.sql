-- MySQL dump 10.13  Distrib 8.0.33, for Win64 (x86_64)
--
-- Host: localhost    Database: chatdb
-- ------------------------------------------------------
-- Server version	8.0.33

/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!50503 SET NAMES utf8mb4 */;
/*!40103 SET @OLD_TIME_ZONE=@@TIME_ZONE */;
/*!40103 SET TIME_ZONE='+00:00' */;
/*!40014 SET @OLD_UNIQUE_CHECKS=@@UNIQUE_CHECKS, UNIQUE_CHECKS=0 */;
/*!40014 SET @OLD_FOREIGN_KEY_CHECKS=@@FOREIGN_KEY_CHECKS, FOREIGN_KEY_CHECKS=0 */;
/*!40101 SET @OLD_SQL_MODE=@@SQL_MODE, SQL_MODE='NO_AUTO_VALUE_ON_ZERO' */;
/*!40111 SET @OLD_SQL_NOTES=@@SQL_NOTES, SQL_NOTES=0 */;

--
-- Table structure for table `all_chat`
--

DROP TABLE IF EXISTS `all_chat`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `all_chat` (
  `message` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `all_chat`
--

LOCK TABLES `all_chat` WRITE;
/*!40000 ALTER TABLE `all_chat` DISABLE KEYS */;
INSERT INTO `all_chat` VALUES ('from: user1, message: hello_everyone!');
/*!40000 ALTER TABLE `all_chat` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `messages_storage`
--

DROP TABLE IF EXISTS `messages_storage`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `messages_storage` (
  `id` int NOT NULL AUTO_INCREMENT,
  `from_id` int DEFAULT NULL,
  `to_id` int DEFAULT NULL,
  `message` varchar(255) DEFAULT NULL,
  `sent_date` date DEFAULT NULL,
  PRIMARY KEY (`id`),
  KEY `from_id` (`from_id`),
  KEY `to_id` (`to_id`),
  CONSTRAINT `messages_storage_ibfk_1` FOREIGN KEY (`from_id`) REFERENCES `users_list` (`id`),
  CONSTRAINT `messages_storage_ibfk_2` FOREIGN KEY (`to_id`) REFERENCES `users_list` (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=6 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `messages_storage`
--

LOCK TABLES `messages_storage` WRITE;
/*!40000 ALTER TABLE `messages_storage` DISABLE KEYS */;
INSERT INTO `messages_storage` VALUES (1,1,9,'hello','2024-10-10'),(2,2,9,'q_hi_test','2024-10-11'),(3,2,1,'qqqqqq','2024-10-11'),(4,2,9,'test_2','2024-10-11'),(5,13,9,'hello','2024-10-11');
/*!40000 ALTER TABLE `messages_storage` ENABLE KEYS */;
UNLOCK TABLES;

--
-- Table structure for table `users_list`
--

DROP TABLE IF EXISTS `users_list`;
/*!40101 SET @saved_cs_client     = @@character_set_client */;
/*!50503 SET character_set_client = utf8mb4 */;
CREATE TABLE `users_list` (
  `id` int NOT NULL AUTO_INCREMENT,
  `login` varchar(255) DEFAULT NULL,
  `password` varchar(60) DEFAULT NULL,
  PRIMARY KEY (`id`),
  UNIQUE KEY `login` (`login`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
/*!40101 SET character_set_client = @saved_cs_client */;

--
-- Dumping data for table `users_list`
--

LOCK TABLES `users_list` WRITE;
/*!40000 ALTER TABLE `users_list` DISABLE KEYS */;
INSERT INTO `users_list` VALUES (1,'aldi','pass'),(2,'user','123'),(9,'player','228'),(12,'test_user','prep'),(13,'user1','12');
/*!40000 ALTER TABLE `users_list` ENABLE KEYS */;
UNLOCK TABLES;
/*!40103 SET TIME_ZONE=@OLD_TIME_ZONE */;

/*!40101 SET SQL_MODE=@OLD_SQL_MODE */;
/*!40014 SET FOREIGN_KEY_CHECKS=@OLD_FOREIGN_KEY_CHECKS */;
/*!40014 SET UNIQUE_CHECKS=@OLD_UNIQUE_CHECKS */;
/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
/*!40111 SET SQL_NOTES=@OLD_SQL_NOTES */;

-- Dump completed on 2024-10-11  4:28:16
