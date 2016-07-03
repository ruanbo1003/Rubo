Create Table If Not Exists register(
	uid INT(11) Primary Key Auto_increment,
	name Varchar(64) Not Null Unique,
	passwd Varchar(256) Not Null,
	time INT(11) Not Null
)Engine=MyIsam Auto_increment=10000 Default Charset=utf8;
