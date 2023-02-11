int case_count = 2;
int esp_matrix_count = 1;
int vertical_box_count = 30;
int horizontal_box_count = 30;

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
    en_pin[0][0] = 14;
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
    signal_pin[1][1] = 12;

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

                    if (percent_value(All_pozition[g][i][j][h], evaluated_data[i][j][h], 25))
                    {
                        total_dogru[g]++;
                        delay(3);
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
        //Serial.println(total_dogru[i]);
        // birden fazla durum olduğunda en yakın durumu seçmeye çalıştığımız yer
        if (total_dogru[i] > max_dogru)
        {
            max_dogru = total_dogru[i];
            // daha sonrasında hangi stringe denk olduğunu anlamak için kaçıncı durum olduğunu saklıyoruz
            max_pozisyon = i;
            //Serial.println(max_dogru);
        }
    }

    // değerlerimizin doğruluğunu sorgulaycaz
    // Serial.print("%d",horizontal_box_count * vertical_box_count * esp_matrix_count  );
    Serial.println(total_dogru[max_pozisyon]);
    if(max_pozisyon == 0)
      Serial.println("yanlis oturuyosun");
    if(max_pozisyon == 1)
      Serial.println("dogru oturuyosun");
    /*
    if (max_dogru >= (horizontal_box_count * vertical_box_count * esp_matrix_count) * 70 / 100)
    {
        
        Serial.println("Oturusun true");
    }
    else
        Serial.println("oturusun False");
        */
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
    all_update(0, "130 101 194 164 225 229 124 44 153 191 157 123 250 205 281 190 218 282 48 0 209 175 288 339 293 281 278 200 221 0 159 101 191 160 209 220 126 44 161 201 169 130 262 227 295 207 241 305 54 0 216 174 281 339 289 271 274 197 222 0 97 55 133 114 157 169 86 16 107 139 110 89 189 160 234 157 168 230 32 0 162 153 224 272 217 203 220 154 164 0 130 82 163 134 207 210 127 36 146 184 147 114 243 183 250 168 210 275 40 0 172 139 237 289 251 240 255 173 188 0 134 86 157 128 184 196 119 27 138 189 148 120 248 180 252 164 211 271 51 0 189 154 261 324 291 272 293 191 204 0 180 126 208 172 214 240 159 54 192 257 207 182 338 248 332 236 286 359 85 0 241 186 293 337 297 259 284 186 198 0 100 62 119 90 130 153 94 9 117 178 122 101 217 136 204 118 164 209 14 0 146 108 203 235 229 194 212 127 143 0 131 90 158 129 158 196 126 23 146 196 141 116 239 165 245 156 191 241 37 0 174 134 232 265 235 215 233 145 161 0 188 133 208 164 191 223 143 44 182 245 187 151 289 203 289 185 229 275 53 0 212 162 272 305 273 247 280 171 192 0 94 61 133 118 113 145 79 8 105 149 110 94 198 163 244 164 173 211 33 0 166 116 215 251 206 181 211 132 138 0 121 79 150 134 133 167 92 13 122 165 130 101 222 166 243 160 179 209 25 0 159 106 190 234 187 161 184 109 121 0 130 86 162 142 146 178 100 21 146 195 170 134 286 211 302 199 233 267 54 0 233 157 263 306 271 238 264 154 178 0 74 39 98 82 80 109 50 0 79 114 91 72 179 125 204 130 160 185 21 0 181 99 186 218 189 154 181 102 115 0 101 64 128 113 115 135 65 2 103 132 105 77 188 139 215 132 160 181 17 0 141 91 174 216 185 150 180 104 122 0 156 103 192 177 160 190 107 23 140 174 147 124 246 225 315 224 214 254 56 2 218 155 250 294 245 208 247 165 174 0 101 68 168 167 130 166 94 24 136 170 142 123 234 235 327 252 222 256 65 2 223 154 240 295 232 197 242 170 164 0 60 24 76 59 64 94 47 0 85 120 91 66 180 120 189 100 141 172 13 0 175 98 158 202 164 123 169 82 98 0 78 42 94 77 79 110 56 0 93 128 103 76 192 124 200 112 143 175 16 0 177 108 177 219 180 141 190 98 115 0 72 33 94 81 69 100 38 0 68 94 72 53 144 110 179 100 122 167 22 0 173 106 174 219 175 141 180 105 114 0 106 58 120 98 101 133 74 4 117 148 121 83 202 139 222 123 170 213 33 0 207 145 218 254 205 173 205 118 138 0 137 74 146 114 124 154 88 12 130 166 140 96 224 152 234 129 163 213 33 0 188 139 213 243 196 170 200 114 136 0 77 39 94 67 76 105 55 0 88 116 100 54 151 99 169 79 103 150 5 0 125 83 152 181 146 131 157 92 111 0 169 95 169 133 150 175 106 25 154 187 162 112 254 186 275 164 193 268 61 0 212 160 254 288 240 227 251 163 184 0 95 51 118 96 107 122 70 6 111 147 119 75 200 144 228 133 148 209 28 0 164 125 204 241 204 187 208 131 136 0 71 26 84 66 86 105 53 0 89 122 99 57 185 124 208 114 133 199 21 0 151 99 171 208 178 164 191 126 125 0 68 36 103 90 89 108 54 0 82 116 86 61 162 133 213 132 128 181 20 0 148 97 171 218 180 157 189 131 121 0 71 40 104 93 98 115 58 2 90 126 88 59 164 123 205 121 128 183 19 0 145 99 172 216 182 157 190 127 122 0 59 26 82 67 83 100 49 0 86 113 80 45 150 100 187 96 107 156 1 0 116 73 139 184 151 125 157 99 106 0 84 45 111 95 107 125 66 6 95 123 94 63 170 131 217 128 127 184 10 0 140 91 166 215 178 145 174 118 118 0 76 38 104 88 95 114 62 11 113 137 98 63 171 124 209 121 128 188 11 0 139 95 164 210 175 148 181 118 117 0 ");
    all_update(1, "250 320 492 541 601 632 684 618 604 556 525 499 445 567 547 390 479 538 346 391 488 517 567 609 611 609 609 461 389 1 278 333 521 574 644 666 665 637 632 622 590 528 468 592 538 406 527 579 389 422 516 538 584 619 626 631 629 529 419 7 341 406 612 706 762 777 746 725 727 714 682 653 561 703 704 520 626 685 499 537 627 668 721 766 775 793 799 685 534 10 367 421 645 739 799 787 693 676 699 741 718 657 604 741 727 553 673 735 533 569 660 705 775 840 856 857 896 756 573 14 355 420 644 740 795 782 680 671 686 715 704 640 599 712 678 525 645 702 506 499 603 642 743 806 838 846 851 765 568 14 349 406 638 737 737 711 621 619 624 657 640 562 555 670 644 499 604 651 446 453 573 622 706 767 801 791 785 691 531 8 254 304 468 562 574 554 486 482 499 514 494 396 454 514 497 373 473 504 339 347 454 491 571 627 646 641 622 558 440 3 245 267 409 464 487 472 413 403 428 432 411 328 394 435 445 309 390 433 294 328 407 438 472 505 519 530 526 529 405 4 266 260 386 426 452 459 401 413 434 438 410 329 388 439 428 286 391 443 320 361 465 482 508 494 513 521 528 495 416 2 223 246 365 415 431 436 401 411 436 437 409 357 381 447 400 312 386 437 325 352 499 530 548 513 493 493 501 434 345 7 253 284 425 488 503 503 453 458 477 486 495 436 507 522 499 383 471 506 363 364 490 523 566 577 576 563 551 449 352 9 255 284 427 489 509 506 453 456 469 475 474 461 473 550 561 402 489 532 388 360 458 483 551 581 598 609 586 447 358 8 173 197 300 358 374 389 338 340 346 347 340 309 362 481 471 298 372 443 299 271 333 343 390 415 437 432 420 334 261 0 74 107 146 171 185 189 163 156 152 151 148 139 154 224 238 141 173 188 126 131 156 157 182 197 202 205 200 163 121 0 220 293 338 379 383 382 324 321 341 339 325 293 361 558 525 342 380 402 315 274 357 372 419 440 447 438 424 381 290 4 315 348 491 566 567 563 509 518 556 522 504 420 561 812 827 685 647 672 554 442 527 550 605 628 634 633 620 596 472 21 190 208 316 382 385 362 281 324 356 352 327 232 318 458 443 352 441 589 496 319 383 403 454 462 463 442 436 397 294 6 244 241 361 468 401 362 300 376 359 357 342 250 337 496 560 385 405 458 350 295 377 412 470 495 515 483 475 426 329 10 177 184 267 342 340 336 272 300 329 413 436 261 291 367 366 253 333 364 301 257 347 366 418 437 436 427 411 384 294 17 147 172 240 317 319 333 288 400 435 376 330 228 262 325 323 216 299 348 283 261 337 367 392 388 392 385 363 331 265 10 160 176 248 314 314 318 280 322 378 347 298 242 268 331 337 224 309 360 300 340 401 458 399 379 367 355 354 307 251 0 150 189 270 318 346 354 313 350 433 392 321 258 260 349 363 263 327 390 351 371 387 421 434 415 394 386 396 350 278 0 250 287 426 483 521 528 467 484 515 526 462 386 377 502 493 390 476 580 478 474 513 561 585 573 577 555 563 488 377 0 242 292 433 508 537 540 470 477 491 495 455 388 394 519 506 404 519 568 466 395 466 501 563 579 595 572 591 468 353 2 240 310 472 547 567 553 481 482 501 506 472 389 441 525 516 459 510 563 451 387 454 487 546 625 630 615 634 502 359 4 252 318 471 518 550 542 475 476 484 487 455 363 409 514 508 453 511 536 431 400 432 460 510 583 643 637 635 525 396 7 198 256 395 414 440 436 382 392 399 407 374 293 296 404 400 320 402 425 356 365 371 384 420 446 472 471 475 405 311 3 194 244 374 391 408 403 359 372 389 398 373 297 303 419 398 314 352 391 325 334 375 401 435 456 457 445 434 356 279 0 238 295 422 473 486 468 423 438 462 467 449 387 392 530 482 378 427 473 380 386 471 501 524 556 544 526 524 417 349 2 333 401 589 664 690 670 641 681 693 688 631 562 551 726 720 546 621 673 524 529 639 674 717 735 723 713 718 590 497 12 ");
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
                    delay(3);
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
            for (int i = 0; i < horizontal_box_count; i++)
            {
                for (int j = 0; j < vertical_box_count; j++)
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
            // MAİN.C DEKİ YERŞ ALABİLİRSİN GEREKİRSE
            for (int i = 0; i < vertical_box_count; i++)
            {
                velo_update(i % 16, 0);
                analogWrite(signal_pin[0][i / 16], 255);
                delay(10);
                for (int j = 0; j < horizontal_box_count; j++)
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
        delay(3);
        string_comparison_accuracy_percentage();
    }
}

// kodumuzda 2 tane kısım vardır
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
