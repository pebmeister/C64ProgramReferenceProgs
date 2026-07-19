10 REM SELF MODIFYING PROGRAM
20 V=1
30 A=2049
40 N=PEEK(A)+PEEK(A+1)*256:IFN=0 THEN END
60 L=PEEK(A+2)+256*PEEK(A+3)
70 A=A+4
80 Q=0
90 C=PEEK(A)
100 IF C=0 THEN A=N:GOTO40
110 IF C=34 THEN Q=1-Q
120 IF Q=1 AND C=32 THEN POKEA,V:V=V+1:IF V=256 THEN END
130 A=A+1
140 GOTO90

1000 PRINT"                "
1010 PRINT"                "
1020 PRINT"                "
1030 PRINT"                "
1040 PRINT"                "
1050 PRINT"                "
1060 PRINT"                "
1070 PRINT"                "
1080 PRINT"                "
1090 PRINT"                "
1100 PRINT"                "
1110 PRINT"                "
1120 PRINT"                "
1130 PRINT"                "
1140 PRINT"                "
1150 PRINT"                "
