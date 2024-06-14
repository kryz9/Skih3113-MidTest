# Skih3113-MidTest

Objective:
Develop a system using an ESP microcontroller (e.g., ESP32, ESP8266) to monitor environmental conditions and store the collected data in a relational database.
Requirements:
•	Sensors:
o	Use at least three different sensors to measure various environmental parameters (e.g., temperature, humidity, light intensity, pressure, CO2).
•	Microcontroller: ESP/Raspberry Pi microcontroller
•	Database and Backend:
o	Store the sensor data in a relational database of your choice (e.g., MySQL, PostgreSQL, SQLite) using any backend of your choice (PHP, Python, NodeJS, etc).
o	Implement proper data schema design to efficiently store sensor readings.
•	Data Acquisition:
o	Read sensor data at regular intervals using appropriate libraries and functions.
o	Implement a suitable delay between sensor readings to avoid overwhelming the ESP and ensure accurate data collection.
•	Data Storage:
o	Connect the ESP to the chosen database and establish a communication channel.
o	Write code to format and transmit sensor readings along with timestamps to the database for storage.
•	Output:
o	Implement a basic web display of the data from database in form of a table, graph, etc.
