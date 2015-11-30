CREATE TABLE `t_sensor_data` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`sensor_id` INT NOT NULL,
	`value` FLOAT NOT NULL,
	`timestamp` DATETIME NOT NULL,
	PRIMARY KEY (`id`)
);

CREATE TABLE `t_equipment` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`name` VARCHAR(16) NOT NULL,
	`location` VARCHAR(16) NOT NULL,
	`info` VARCHAR(64),
	PRIMARY KEY (`id`)
);

CREATE TABLE `t_sensor_info` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`equipment_id` INT NOT NULL,
	`name` VARCHAR(16) NOT NULL,
	`units` VARCHAR(6) NOT NULL,
	`info` VARCHAR(64),
	PRIMARY KEY (`id`)
);

CREATE TABLE `t_setpoint_data` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`setpoint_id` INT NOT NULL,
	`value` FLOAT NOT NULL,
	`timestamp` DATETIME NOT NULL,
	PRIMARY KEY (`id`)
);

CREATE TABLE `t_setpoint_info` (
	`id` INT NOT NULL AUTO_INCREMENT,
	`equipment_id` INT NOT NULL,
	`name` VARCHAR(16) NOT NULL,
	`units` VARCHAR(6) NOT NULL,
	`info` VARCHAR(64),
	PRIMARY KEY (`id`)
);

ALTER TABLE `t_sensor_data` ADD CONSTRAINT `t_sensor_data_fk0` FOREIGN KEY (`sensor_id`) REFERENCES `t_sensor_info`(`id`);

ALTER TABLE `t_sensor_info` ADD CONSTRAINT `t_sensor_info_fk0` FOREIGN KEY (`equipment_id`) REFERENCES `t_equipment`(`id`);

ALTER TABLE `t_setpoint_data` ADD CONSTRAINT `t_setpoint_data_fk0` FOREIGN KEY (`setpoint_id`) REFERENCES `t_setpoint_info`(`id`);

ALTER TABLE `t_setpoint_info` ADD CONSTRAINT `t_setpoint_info_fk0` FOREIGN KEY (`equipment_id`) REFERENCES `t_equipment`(`id`);

