 --�����ݿ����½�һ���ݿ�new_db
 --��new_db���½�һ��student
 CREATE TABLE Student
  ( Sno CHAR(9) PRIMARY KEY, 
   Sname CHAR(20) UNIQUE, 
   Ssex CHAR(2), 
   Sage SMALLINT,
   Sdept CHAR(20));
   
 --��student���������
 insert into Student(Sno,Sname,Ssex,Sage,Sdept)
  values('200215121','����','��',20,'CS');
insert into Student(Sno,Sname,Ssex,Sage,Sdept) 
values('200215122','����','Ů',19,'CS');
insert into Student(Sno,Sname,Ssex,Sage,Sdept)
 values('200215123','����','Ů',18,'MA');
insert into Student(Sno,Sname,Ssex,Sage,Sdept)
 values('200215125','����','��',19,'IS');
 
--����һ����ΪStudent_Info���޲����洢���̣��ù���ʵ�ִӱ��в�ѯ����ѧ��������Ϣ��
CREATE PROCEDURE Student_Info
AS
SELECT * 
FROM Student
--ִ��Student_Info�洢����
EXECUTE Student_Info

/*����һ����ΪStudent_CS���޲����洢���̣�
�ô洢����ʵ�ִӱ��в�ѯ���м����ϵ��ѧ������*/
CREATE PROCEDURE Student_CS
AS
SELECT Sname 
FROM Student 
where Sdept='CS'

--ִ��Student_CS�洢����
EXECUTE Student_CS

--�����������Ĵ洢���̡����ָ��ѧ�ŵ�ѧ����Ϣ��
CREATE PROCEDURE Query_Student_Info
@Sno char(9)
AS
SELECT * FROM Student WHERE Sno=@Sno
--ִ�д洢����Query_Student_Info
EXECUTE Query_Student_Info '200215121'


/*�����������Ĵ洢���̡�����ָ����������ѧ����Ϣ��
���ѧ������Ѿ������������ӡ�*/
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
      PRINT '����ɹ���'
END
ELSE
     PRINT '������󣬸�ѧ�ŵ�ѧ����Ϣ�����ݿ����Ѿ����ڣ�'
     
--ִ�д洢����Add_Student������ֱ���ʲô��
EXECUTE Add_Student '200215121', '����','��',20,'CS'
go
EXECUTE Add_Student '200215126', '����','��',20,'CS'

--��������������Ĵ洢���̡���д�洢���̣�����ָ��ѧ��ѧ�ŵ�ѧ������
CREATE PROCEDURE Student_Name
@Sno char(9),
@Sname char(20) OUTPUT
AS
SELECT @Sname=Sname FROM Student WHERE Sno=@Sno

--�������
Declare @Sname char(20)
EXECUTE Student_Name  '200215121' ,@Sname OUTPUT 
Select   @Sname AS ѧ������

--�����洢����Student_Delete��ɾ��ָ��ѧ�ŵ�ѧ����Ϣ��
--���ɹ����ӡ��ɾ���ɹ�������û�и�ѧ�ţ����ӡ���޸�ѧ������

CREATE PROCEDURE Student_Delete
@Sno char(9),
@Flag int=0
AS
SELECT @Flag=count(*) FROM Student WHERE Sno=@Sno
IF @Flag=1
BEGIN
      DELETE FROM Student WHERE Sno=@Sno

      PRINT 'ɾ���ɹ���'
END
ELSE
     PRINT '�޸�ѧ��'
--ʹ��ɾ��ѧ��200215127��200215121�ֱ���֤�洢����Student_Delete.
EXECUTE Student_Delete '200215127'
go
EXECUTE Student_Delete '200215121'