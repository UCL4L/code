 --在数据库中新建一数据库new_db
 --在new_db中新建一表student
 CREATE TABLE Student
  ( Sno CHAR(9) PRIMARY KEY, 
   Sname CHAR(20) UNIQUE, 
   Ssex CHAR(2), 
   Sage SMALLINT,
   Sdept CHAR(20));
   
 --在student中添加数据
 insert into Student(Sno,Sname,Ssex,Sage,Sdept)
  values('200215121','李勇','男',20,'CS');
insert into Student(Sno,Sname,Ssex,Sage,Sdept) 
values('200215122','刘晨','女',19,'CS');
insert into Student(Sno,Sname,Ssex,Sage,Sdept)
 values('200215123','王敏','女',18,'MA');
insert into Student(Sno,Sname,Ssex,Sage,Sdept)
 values('200215125','张立','男',19,'IS');
 
--创建一个名为Student_Info的无参数存储过程，该过程实现从表中查询所有学生基本信息。
CREATE PROCEDURE Student_Info
AS
SELECT * 
FROM Student
--执行Student_Info存储过程
EXECUTE Student_Info

/*创建一个名为Student_CS的无参数存储过程，
该存储过程实现从表中查询所有计算机系的学生姓名*/
CREATE PROCEDURE Student_CS
AS
SELECT Sname 
FROM Student 
where Sdept='CS'

--执行Student_CS存储过程
EXECUTE Student_CS

--创建带参数的存储过程。输出指定学号的学生信息。
CREATE PROCEDURE Query_Student_Info
@Sno char(9)
AS
SELECT * FROM Student WHERE Sno=@Sno
--执行存储过程Query_Student_Info
EXECUTE Query_Student_Info '200215121'


/*创建带参数的存储过程。根据指定参数增加学生信息，
如果学生编号已经存在则不能增加。*/
CREATE PROCEDURE Add_Student
@Sno char(9),
@Sname char(20),
@Ssex char(2),
@Sage smallint,
@Sdept char(20),
@Flag int=0
AS
SELECT @Flag=count(*) FROM Student WHERE Sno=@Sno
IF @Flag=0
BEGIN
      INSERT INTO Student
      VALUES(@SNO, @Sname,@Ssex, @Sage,@Sdept)
      PRINT '插入成功！'
END
ELSE
     PRINT '插入错误，该学号的学生信息在数据库中已经存在！'
     
--执行存储过程Add_Student，结果分别是什么？
EXECUTE Add_Student '200215121', '李勇','男',20,'CS'
go
EXECUTE Add_Student '200215126', '李勇','男',20,'CS'

--创建带输出参数的存储过程。编写存储过程，返回指定学生学号的学生姓名
CREATE PROCEDURE Student_Name
@Sno char(9),
@Sname char(20) OUTPUT
AS
SELECT @Sname=Sname FROM Student WHERE Sno=@Sno

--测试语句
Declare @Sname char(20)
EXECUTE Student_Name  '200215121' ,@Sname OUTPUT 
Select   @Sname AS 学生姓名

--创建存储过程Student_Delete，删除指定学号的学生信息，
--若成功则打印‘删除成功’，若没有该学号，则打印‘无该学生’。

CREATE PROCEDURE Student_Delete
@Sno char(9),
@Flag int=0
AS
SELECT @Flag=count(*) FROM Student WHERE Sno=@Sno
IF @Flag=1
BEGIN
      DELETE FROM Student WHERE Sno=@Sno

      PRINT '删除成功！'
END
ELSE
     PRINT '无该学生'
--使用删除学号200215127，200215121分别验证存储过程Student_Delete.
EXECUTE Student_Delete '200215127'
go
EXECUTE Student_Delete '200215121'