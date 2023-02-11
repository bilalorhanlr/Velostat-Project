int case_count = 2;
int esp_matrix_count = 1;
int vertical_box_count = 30;
int horizontal_box_count = 27;

//              [bizim koyduğumuz durumlar][var olabilecek esp sayısı][değerler][değerler]
int All_pozition[2][1][30][30];
int evaluated_data[1][30][30];
char all_cases_str[2][100];

// durum 0 argüman
int main_data[1][30][30];

int temp[6][1][30][30]; //[6]-> her bir while dongusu
                        //[2]->alt matris ust matris
                        //[28][28] değerlerimiz

char str_pin[4]; // 0000

void str_pin_upload(const char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str_pin[i] = str[i];
    }
    str_pin[4] = '\0';
}

void pin(int a)
{
    if (a == 0)
        str_pin_upload("0000");
    if (a == 1)
        str_pin_upload("1000");
    if (a == 2)
        str_pin_upload("0100");
    if (a == 3)
        str_pin_upload("1100");
    if (a == 4)
        str_pin_upload("0010");
    if (a == 5)
        str_pin_upload("1010");
    if (a == 6)
        str_pin_upload("0110");
    if (a == 7)
        str_pin_upload("1110");
    if (a == 8)
        str_pin_upload("0001");
    if (a == 9)
        str_pin_upload("1001");
    if (a == 10)
        str_pin_upload("0101");
    if (a == 11)
        str_pin_upload("1101");
    if (a == 12)
        str_pin_upload("0011");
    if (a == 13)
        str_pin_upload("1011");
    if (a == 14)
        str_pin_upload("0111");
    if (a == 15)
        str_pin_upload("1111");
}

// pinleri aldık
int connect_pin[2][4];
int en_pin[2][2];
int signal_pin[2][2];

void setup()
{
    // tek tek yazılacak
    connect_pin[0][0] = 23; // s0
    connect_pin[0][1] = 22; // s1
    connect_pin[0][2] = 21; // s2
    connect_pin[0][3] = 19; // s3
    en_pin[0][0] = 12;
    en_pin[0][1] = 17;
    signal_pin[0][0] = 33;
    signal_pin[0][1] = 13;

    connect_pin[1][0] = 4;
    connect_pin[1][1] = 26;
    connect_pin[1][2] = 27;
    connect_pin[1][3] = 18;
    en_pin[1][0] = 32;
    en_pin[1][1] = 16;
    signal_pin[1][0] = 25;
    signal_pin[1][1] = 14;

    pinMode(connect_pin[0][0], OUTPUT);
    pinMode(connect_pin[0][1], OUTPUT);
    pinMode(connect_pin[0][2], OUTPUT);
    pinMode(connect_pin[0][3], OUTPUT);
    pinMode(en_pin[0][0], OUTPUT);
    pinMode(en_pin[0][1], OUTPUT);
    pinMode(signal_pin[0][0], OUTPUT);
    pinMode(signal_pin[0][1], OUTPUT);
    pinMode(connect_pin[1][0], OUTPUT);
    pinMode(connect_pin[1][1], OUTPUT);
    pinMode(connect_pin[1][2], OUTPUT);
    pinMode(connect_pin[1][3], OUTPUT);
    pinMode(en_pin[1][0], OUTPUT);
    pinMode(en_pin[1][1], OUTPUT);
    pinMode(signal_pin[1][0], INPUT);
    pinMode(signal_pin[1][1], INPUT);

    Serial.begin(9600);
    delay(1000);
    digitalWrite(en_pin[0][0], LOW);
    digitalWrite(en_pin[0][1], LOW);
    digitalWrite(en_pin[1][0], LOW);
    digitalWrite(en_pin[1][1], LOW);
}

int n;
// stringi sayıya çevirmek için kullanılan fonksiyon
int ft_atoi(const char *str)
{
    int neg_value;
    long value;

    neg_value = 1;
    value = 0;
    // boşluk tab vb şeyleri atlıyoruz
    while (str[n] == ' ' || str[n] == '\n' || str[n] == '\t' || str[n] == '\v' || str[n] == '\f' || str[n] == '\r')
        n++;
    if (str[n] == '-' || str[n] == '+')
    {
        if (str[n++] == '-')
            neg_value *= -1;
    }
    // sayı ya asıl çevirdiğimiz yer
    while (str[n] >= '0' && str[n] <= '9')
    {
        value = value * 10 + (str[n++] - 48);
        if ((value * neg_value) < -2147483648)
            return (0);
        if ((value * neg_value) > 2147483647)
            return (-1);
    }
    return (value * neg_value);
}

int low_high(char a)
{
    if (a == '1')
        return (HIGH);
    return (LOW);
}
//               7      //  0
void velo_update(int a, int alt_ust)
{
    // pinimiz kaç ise onu stringe çevirdik
    pin(a);
    // pini seçebilmek için low high yaptık
    digitalWrite(connect_pin[alt_ust][0], low_high(str_pin[0]));
    digitalWrite(connect_pin[alt_ust][1], low_high(str_pin[1]));
    digitalWrite(connect_pin[alt_ust][2], low_high(str_pin[2]));
    digitalWrite(connect_pin[alt_ust][3], low_high(str_pin[3]));
}

void string_update(const char *string, int kacinci_durum)
{
    for (int i = 0; string[i] != '\0'; i++)
        all_cases_str[kacinci_durum][i] = string[i];
}
// elde olan sayının yüzde x i kadar altı veya üstünde ise 0 döndürür o aralıkta ise 1 döndürür
int percent_value(int elde_olan, int istenen, int yuzde)
{
    if ((istenen - (istenen * yuzde / 100)) < elde_olan && (istenen + (istenen * yuzde / 100)) > elde_olan)
        return (1);
    return (0);
}
// elde olan veri ile daha önce girmiş olduğumuz veriyi karşılaştırır
void string_comparison_accuracy_percentage()
{
    // elde edilen matris All_pozitiondaki matrisle karşılaştırılıp yuzde x uzeri yakın olan seçenekler ona denk kabul edilecek
    //  mesala 3,5 noktası dik pozisyonunda 600 ise yuzde 20 sapma payı verirsek 480 - 720 arasında bir değer geldiğinde o kutucuk doğru kabul edilir
    //  elimizdeki kutucukların %x kadarı doğru ise pozisyonumuz diktir teşhisini yaparız
    int total_dogru[case_count];
    for (int i = 0; i < case_count; i++)
    {
        total_dogru[i] = 0;
    }

    for (int i = 0; i < esp_matrix_count; i++)
    {
        for (int j = 0; j < vertical_box_count; j++)
        {
            for (int h = 0; h < horizontal_box_count; h++)
            {
                for (int g = 0; g < case_count; g++)
                {

                    if (percent_value(All_pozition[g][0][j][h], evaluated_data[0][j                      ][h], 20))
                    {
                        // Serial.print(All_pozition[0][i][j][h]);
                        // delay(3);
                        // Serial.print(evaluated_data[i][j][h]);
                        total_dogru[g]++;
                    }
                }
            }
        }
        //Serial.print(total_dogru[i]);
        delay(3);
    }
    // burada hangi i değerine geldiğine gore bir print işlemi yapılır ve kullanıcı bilgilendirilir
    int max_dogru = 0;
    int max_pozisyon = 0;
    for (int i = 0; i < case_count; i++)
    {
        // birden fazla durum olduğunda en yakın durumu seçmeye çalıştığımız yer
        if (total_dogru[i] > max_dogru)
        {
            max_dogru = total_dogru[i];
            // daha sonrasında hangi stringe denk olduğunu anlamak için kaçıncı durum olduğunu saklıyoruz
            max_pozisyon = i;
            // Serial.print("%d\n",max_dogru);
        }
    }
    // değerlerimizin doğruluğunu sorgulaycaz
    // Serial.print("%d",horizontal_box_count * vertical_box_count * esp_matrix_count  );
        Serial.println(total_dogru[max_pozisyon]);
    if(max_pozisyon == 0)
      Serial.println("yanlis oturuyosun");
    if(max_pozisyon == 1)
      Serial.println("dogru oturuyosun");
}

void all_update(int durum, char *veri)
{
    n = 0;
    int j = 0;
    for (int k = 0; k < esp_matrix_count; k++)
    {
        for (int i = 0; i < vertical_box_count; i++)
        {
            for (int j = 0; j < horizontal_box_count; j++)
            {
                // veri bir string olduğu için stringin içerisinden tek tek sayıya çeviriyoruz
                All_pozition[durum][k][i][j] = ft_atoi(veri);
            }
        }
    }
}

void degerlendicek_update(int ***veri)
{
    for (int k = 0; k < esp_matrix_count; k++)
    {
        for (int i = 0; i < vertical_box_count; i++)
        {
            for (int j = 0; horizontal_box_count > j; j++)
            {
                // daha sonra karşılaştırma yapacağımız veriyi alıyoruz
                evaluated_data[k][i][j] = veri[k][i][j];
            }
        }
    }
}

void setup2()
{
    // durum adlandirma
    string_update("tamamen duzgun", 0);
    string_update("yarim dogru", 1);
    all_update(0, "992 1050 1043 1059 1094 1082 1052 1048 1059 1020 1047 1172 1131 1128 1125 1049 675 833 850 873 748 860 891 897 949 938 862 900 937 960 973 980 1016 959 953 944 943 972 1046 1022 1005 993 876 613 775 796 805 703 823 875 898 920 933 816 905 908 935 968 972 970 939 922 940 928 992 1003 952 967 980 897 613 786 808 816 713 793 830 843 882 921 815 1008 1015 1034 1084 1106 1090 1101 1060 1049 1046 1133 1137 1087 1065 1075 1000 695 869 898 903 777 889 909 913 943 944 877 1167 1191 1245 1247 1296 1288 1280 1332 1274 1201 1300 1322 1294 1277 1256 1180 866 1091 1111 1111 923 1071 1084 1080 1112 1148 1037 1045 1049 1076 1141 1162 1182 1111 1158 1062 1053 1064 1060 1092 1091 1079 1000 759 965 994 979 814 912 918 933 963 971 882 1084 1052 1101 1155 1130 1136 1154 1144 1082 1048 1067 1063 1093 1078 1055 1015 809 980 991 971 823 920 953 962 990 995 882 1204 1161 1169 1182 1206 1200 1216 1206 1183 1125 1133 1143 1174 1187 1190 1116 891 1041 1060 1058 973 1028 1028 1020 1082 1083 987 1170 1192 1215 1240 1254 1277 1265 1241 1233 1207 1235 1221 1267 1275 1272 1183 900 1100 1135 1147 1020 1064 1053 1073 1110 1099 1024 1304 1313 1321 1323 1356 1389 1379 1366 1316 1293 1292 1353 1315 1340 1329 1264 942 1129 1164 1205 1022 1145 1177 1192 1231 1181 1091 1583 1587 1575 1592 1606 1671 1548 1560 1546 1504 1489 1590 1587 1588 1563 1507 1108 1315 1349 1369 1180 1348 1389 1409 1424 1413 1288 1679 1674 1695 1703 1739 1776 1751 1691 1664 1661 1673 1768 1767 1756 1746 1652 1208 1503 1519 1495 1267 1475 1531 1514 1548 1541 1429 1473 1464 1481 1496 1555 1594 1587 1526 1487 1507 1489 1596 1580 1601 1582 1477 1056 1290 1328 1353 1193 1336 1339 1320 1372 1361 1268 1570 1559 1578 1612 1698 1696 1688 1606 1636 1587 1610 1626 1575 1621 1613 1523 1088 1346 1387 1394 1178 1362 1402 1436 1531 1478 1363 2467 2447 2453 2476 2557 2536 2498 2465 2442 2399 2408 2713 2716 2690 2664 2555 1716 2034 2074 2106 1838 2099 2121 2104 2186 2179 2063 2465 2471 2471 2486 2530 2519 2487 2499 2456 2440 2437 2726 2724 2708 2686 2564 1733 2047 2091 2124 1839 2110 2147 2142 2210 2190 2062 1446 1413 1440 1445 1461 1458 1442 1452 1400 1405 1411 1570 1591 1572 1540 1488 1004 1217 1247 1270 1052 1261 1260 1239 1283 1275 1214 1799 1758 1758 1776 1801 1789 1784 1783 1731 1707 1711 1800 1811 1803 1805 1803 1232 1473 1508 1565 1270 1513 1496 1470 1540 1549 1507 1852 1845 1854 1857 1875 1893 1873 1871 1811 1793 1757 1871 1923 1858 1829 1780 1274 1529 1555 1604 1305 1585 1582 1550 1604 1575 1528 1756 1763 1754 1744 1752 1783 1735 1777 1733 1699 1645 1754 1801 1787 1747 1685 1217 1441 1456 1506 1237 1488 1493 1489 1543 1539 1485 1613 1593 1577 1609 1608 1612 1585 1526 1519 1567 1480 1574 1594 1574 1583 1516 1047 1249 1281 1324 1114 1346 1357 1345 1399 1367 1311 1760 1779 1787 1751 1761 1762 1784 1762 1755 1746 1740 1865 1800 1780 1769 1704 1187 1389 1424 1485 1249 1524 1557 1531 1581 1552 1477 1714 1762 1757 1742 1779 1798 1742 1737 1737 1750 1733 1860 1802 1798 1794 1687 1142 1397 1435 1479 1240 1512 1507 1483 1557 1544 1477 1615 1647 1680 1730 1721 1748 1674 1682 1662 1680 1685 1780 1731 1703 1675 1607 1055 1313 1365 1439 1128 1416 1441 1443 1486 1463 1401 1591 1575 1584 1591 1607 1615 1578 1583 1553 1558 1532 1710 1621 1612 1582 1532 1009 1220 1250 1294 1051 1358 1403 1351 1398 1385 1306 1481 1470 1483 1515 1514 1500 1450 1459 1462 1441 1449 1530 1571 1513 1490 1414 947 1164 1181 1211 1004 1240 1240 1227 1278 1293 1223 1193 1191 1219 1231 1240 1204 1296 1286 1215 1219 1254 1270 1286 1246 1255 1170 795 986 995 1016 840 1023 1032 1014 1064 1063 1050 1038 1083 1079 1088 1089 1084 1072 1120 1080 1052 1049 1110 1113 1080 1065 1004 665 795 827 866 725 863 859 867 915 906 858 1256 1276 1271 1293 1261 1237 1219 1270 1239 1190 1214 1280 1296 1249 1231 1180 789 962 981 1003 816 1003 1034 1038 1082 1081 1008 1210 1243 1230 1222 1201 1226 1175 1248 1173 1197 1182 1261 1334 1236 1224 1164 789 958 976 1021 893 1043 1044 994 1017 1042 988 ");
    all_update(1, "1164 1187 1230 1218 1251 1248 1237 1222 1215 1190 1238 1365 1328 1328 1314 1258 839 1006 1037 1052 931 1078 1094 1097 1151 1131 1076 1013 1047 1092 1091 1103 1149 1103 1106 1085 1083 1131 1187 1185 1193 1141 1059 753 922 941 942 847 981 1029 1037 1067 1068 981 933 947 963 1000 1020 1020 1002 978 985 970 1054 1079 1065 1062 1067 1004 720 871 887 892 798 886 914 929 962 1013 921 1066 1074 1095 1158 1170 1162 1169 1133 1126 1095 1199 1231 1185 1174 1177 1127 821 975 1002 998 888 1001 1023 1022 1050 1054 1008 1203 1221 1279 1265 1324 1330 1328 1324 1301 1233 1344 1377 1361 1359 1331 1274 941 1148 1166 1175 1000 1148 1163 1160 1198 1224 1150 1067 1061 1081 1158 1171 1187 1126 1150 1081 1073 1092 1104 1137 1135 1125 1067 781 979 1013 1013 877 967 964 986 1028 1024 953 1049 1022 1077 1129 1112 1108 1136 1119 1075 1041 1058 1065 1091 1086 1064 1037 812 969 994 972 842 946 976 982 1007 1005 942 1088 1074 1093 1121 1146 1136 1149 1129 1134 1086 1091 1097 1132 1152 1170 1117 876 1010 1030 1013 940 1018 1008 1007 1052 1063 1021 1172 1196 1229 1256 1274 1302 1299 1261 1265 1243 1262 1249 1305 1314 1323 1260 926 1119 1172 1169 1052 1128 1116 1138 1175 1168 1130 1329 1331 1349 1366 1411 1439 1434 1407 1371 1344 1344 1402 1373 1403 1409 1353 983 1173 1206 1233 1078 1213 1242 1276 1300 1267 1217 1601 1592 1584 1622 1634 1691 1589 1592 1591 1558 1519 1637 1649 1596 1609 1596 1139 1361 1419 1438 1254 1422 1465 1486 1505 1488 1422 1747 1736 1759 1793 1825 1858 1838 1760 1750 1729 1786 1884 1878 1872 1863 1786 1302 1617 1667 1614 1389 1634 1666 1647 1688 1677 1604 1554 1555 1576 1610 1651 1683 1682 1615 1598 1604 1601 1701 1681 1717 1691 1624 1130 1376 1427 1447 1308 1469 1461 1460 1508 1494 1433 1580 1593 1605 1649 1728 1715 1718 1624 1673 1605 1642 1679 1653 1696 1679 1625 1131 1394 1450 1460 1270 1456 1486 1525 1595 1556 1485 2566 2555 2588 2624 2701 2673 2654 2614 2609 2570 2592 2893 2904 2888 2863 2782 1840 2191 2254 2301 2044 2335 2352 2341 2418 2401 2333 2540 2536 2562 2605 2655 2643 2617 2618 2598 2588 2603 2889 2898 2889 2866 2772 1826 2176 2243 2283 2013 2330 2355 2355 2417 2396 2312 1388 1377 1391 1420 1440 1427 1409 1411 1385 1386 1385 1530 1564 1540 1525 1503 970 1189 1235 1258 1058 1282 1284 1266 1298 1295 1268 1699 1672 1674 1712 1735 1711 1708 1693 1684 1665 1660 1761 1781 1754 1792 1790 1166 1421 1478 1535 1274 1535 1510 1500 1550 1553 1542 1829 1821 1850 1865 1875 1886 1870 1869 1834 1833 1790 1908 1933 1913 1895 1854 1241 1533 1582 1636 1363 1658 1658 1635 1683 1632 1626 1687 1694 1712 1705 1720 1737 1699 1728 1697 1704 1645 1743 1793 1792 1753 1713 1148 1402 1448 1503 1255 1528 1543 1534 1575 1564 1547 1543 1532 1528 1589 1589 1574 1543 1531 1511 1566 1484 1580 1603 1589 1595 1552 1013 1231 1290 1330 1142 1394 1397 1399 1440 1400 1375 1719 1750 1782 1756 1785 1786 1805 1778 1778 1771 1774 1889 1850 1840 1832 1790 1211 1436 1477 1545 1311 1617 1649 1629 1657 1634 1600 1713 1761 1783 1781 1821 1843 1783 1781 1774 1787 1801 1919 1890 1881 1882 1802 1188 1451 1501 1556 1333 1625 1615 1605 1675 1651 1613 1661 1688 1727 1784 1789 1806 1756 1760 1732 1750 1783 1885 1852 1816 1779 1731 1122 1397 1456 1543 1264 1566 1592 1594 1641 1600 1555 1518 1512 1537 1564 1586 1593 1560 1567 1550 1554 1559 1717 1658 1648 1611 1578 1032 1252 1287 1340 1124 1429 1469 1433 1471 1454 1412 1428 1435 1457 1505 1501 1497 1453 1473 1476 1459 1490 1581 1628 1586 1549 1494 986 1207 1247 1284 1098 1355 1339 1323 1372 1372 1325 1182 1196 1243 1260 1286 1250 1335 1325 1270 1283 1361 1352 1348 1407 1357 1272 854 1046 1058 1089 935 1160 1156 1140 1176 1184 1168 997 1050 1047 1061 1071 1067 1067 1122 1096 1104 1053 1100 1120 1100 1086 1064 682 798 813 884 768 918 918 928 965 956 935 1189 1214 1226 1243 1233 1221 1206 1256 1235 1196 1230 1284 1318 1268 1255 1217 816 983 1013 1047 890 1087 1113 1110 1129 1124 1101 1202 1213 1229 1236 1215 1244 1205 1279 1208 1260 1246 1305 1354 1282 1282 1239 842 1008 1034 1077 953 1134 1135 1096 1118 1128 1111 ");
    }

void loop()
{
    int setup_first = 1;
    //int mod = 0; //-> veri toplama
    int mod = 1; //-> veri işleme
    if (mod == 0)
    {
        int f = 0;
        int alt_ust = 0;
        while (f < 6)
        {
            for (int i = 0; i < vertical_box_count; i++)
            {
                velo_update(i % 16, 0);                  // hangi pini kullancaksam seçtim
                analogWrite(signal_pin[0][i / 16], 255); // seçilen pine tam güç verildi
                delay(10);
                for (int j = 0; j < horizontal_box_count; j++)
                {
                    velo_update(j % 16, 1);
                    temp[f][0][i][j] = analogRead(signal_pin[1][j / 16]);
                    delay(5);
                }
                analogWrite(signal_pin[0][i / 16], 0);
                delay(10);

            }
            f++;
            
        }
        delay(3);
        // verileri işle
        for (int k = 0; k < 1; k++)
        {
            for (int i = 0; i < vertical_box_count; i++)
            {
                for (int j = 0; j < horizontal_box_count; j++)
                {
                    main_data[k][i][j] = (temp[0][k][i][j] + temp[1][k][i][j] + temp[2][k][i][j] + temp[3][k][i][j] + temp[4][k][i][j] + temp[5][k][i][j]) / 6;
                    Serial.print(main_data[k][i][j]);
                    Serial.print(" ");
                }
            }  
        }
        // verileri print et
        // printt_dizi(esp_matrix_count, vertical_box_count, horizontal_box_count);
    }
    if (mod == 1)
    {
        // ALL_UPDATE fonksiyonu kullanılarak daha once elde edilen veriler all_update doldurulur
        if (setup_first++)
            setup2();
        int f = 0;
        int alt_ust = 0;
        while (f < 1)
        {
            // MAİN.C DEKİ YERİ ALABİLİRSİN GEREKİRSE
            for (int i = 0; i < horizontal_box_count; i++)
            {
                velo_update(i % 16, 0);
                analogWrite(signal_pin[0][i / 16], 255);
                delay(10);
                for (int j = 0; j < vertical_box_count; j++)
                {
                    velo_update(j % 16, 1);
                    evaluated_data[0][i][j] = analogRead(signal_pin[1][j / 16]);
                    delay(3);
                }
                analogWrite(signal_pin[0][i / 16], 0);
                delay(10);
            }
            f++;
        }
        Serial.print("verilerimiz alindi");
        delay(3);
        string_comparison_accuracy_percentage();
    }
}

// kodumda 2 tane kısım vardır
//  1.kısımda veri alma kısmı mesala dik doğru poizsyonunun verilerini alacağız ve daha sonra bu verileri programımıza dahil edip 2.kısıma geçeceğiz
//  yani basit bir şekilde örnek vermek gerekirse 1.kısımda 50 değerini dik doğru pozisyon durumu için aldıysam programıma bunu el ile girip 2.kısımda
//  çalıştıracağım ve tekrar programım 50 değerine yakın bir değer verdiğinde ben şunu anlamış olacağım bu dik doğru pozisyon durumudur
//  bunu her durum için istediğimiz sayıda yapabiliriz , yanlış bir pozisyon içinde, spesifik bir durum içinde, kaç tane durum olacağı tamamen bize kalmış

// verileri alırken daha stabil veri alabilmek için 6 kerek aynı noktaların (karelerin) değerlerini alır ve her karenin ortalamalarını
// kaydederiz

// 2.kısımda programımızın hangi durumda olduğuna karar verebilmek için elimizdeki her durum matrisiyle bir karşılaştırma yapıp
// hangi duruma denk olduğunu anlamamız gerekir burada uyguladığımız yöntem her bir durum matrisini kutucuğunun o anlık verilerler karşılaştırılıp
// yüzde olarak en çok hangisine benzer ise o durum matrisi gerçekleştirildiği varsayılır burada yüzde oranları keyfimize ve programın verimliliğine bağlı
// değiştirilecektir daha kesin sonuçlar elde edebilmek için
// her bir kutucuğun yüzde olarak değerleri karşılaştırıldıktan sonra tüm kutucukların sayısının yüzde x i kadarı yakın olması gerekir mesala elimizde 2 durum var
// birinci durumla benzer kutucukların sayısı 300 2.durumla 400 ise 2.durum seçilir
