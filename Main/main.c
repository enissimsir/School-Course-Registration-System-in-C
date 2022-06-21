#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct Ogrenciler{
	int numara;
	char *ad;
	int ders_sayisi;
	int toplam_kredi;
	struct Ogrenciler *next;
	struct Ogrenciler *prev;
}OGRENCI;

typedef struct Dersler{
	char *kod;
	char *ad;
	int kapasite;
	int kredi;
	int ogrenci_sayisi;
	int *numaralar;
	struct Dersler *next;
}DERS;

typedef struct Kayitlar{
	int id;
	char *kod;
	int no;
	int gun, ay, yil;
	char *durum;
	struct Kayitlar *next;
}KAYIT;

void ders_ekle(DERS **head,char kod[10],char ad[40],int kapasite,int kredi);
void ders_cikar(DERS **head,char kod[10],KAYIT **head_kayit,int gun,int ay,int yil);
void ders_yazdir(DERS *head);
void ogrenci_ekle(OGRENCI **head,int numara,char ad[30]);
void ogrenci_cikar(OGRENCI **head,int numara);
void ogrenci_yazdir(OGRENCI *head);
void kayit_ekle(KAYIT **head,char kod[10],int no,int gun,int ay,int yil,char durum[15]);
void kayit_yazdir(KAYIT *head);
void derse_kayit(DERS **head,int no,char kod[10],KAYIT **head_kayit,int gun,int ay,int yil,OGRENCI **head_ogrenci,int max_ders,int max_kredi);
void dersi_birak(DERS **head,int no,char kod[10],KAYIT **head_kayit,int gun,int ay,int yil,OGRENCI **head_ogrenci);
void sinif_listeleri(DERS *head);
void ders_programlari(OGRENCI *head,DERS *head_ders);
void ders_dosyasini_oku(DERS **head);
void ogrenci_dosyasini_oku(OGRENCI **head);

int main(int argc, char *argv[]) {
	time_t t = time(NULL);
  	struct tm tm = *localtime(&t);
  	int gun=tm.tm_mday, ay=tm.tm_mon + 1, yil=tm.tm_year + 1900;
	DERS *head_ders=NULL;
	OGRENCI *head_ogrenci=NULL;
	KAYIT *head_kayit=NULL;
	int secim, i, max_ders, max_kredi;
	ders_dosyasini_oku(&head_ders); ogrenci_dosyasini_oku(&head_ogrenci);
	sinif_listeleri(head_ders); ders_programlari(head_ogrenci,head_ders);
	printf("Ders kayit sistemine hosgeldiniz!\nLutfen oncelikle bir ogrencinin alabilecegi maximum ders sayisini ve maximum kredi miktarini giriniz:");
	scanf("%d%d",&max_ders,&max_kredi);
	do{
		printf("\nLutfen yapmak istediginiz islemi secin.\nDers ekleme/silme islemleri icin: 1\nOgrenci ekleme/silme islemleri icin: 2\nDers kayit islemleri icin: 3\n");
		scanf("%d",&secim);
		if(secim==1){
			char kod[10];
			char ad[40];
			int kapasite, kredi;
			while(secim==1){
				printf("Ders eklemek icin 1'i, silmek icin 2'yi tuslayin: "); scanf("%d",&secim); while ((getchar()) != '\n');
				if(secim==1){
					printf("Dersin kodu: "); fgets(kod, 10, stdin); kod[strcspn(kod, "\n")] = 0;
					printf("Dersin adi: "); fgets(ad, 40, stdin); ad[strcspn(ad, "\n")] = 0;
					printf("Dersin kapasitesi: "); scanf("%d",&kapasite);
					printf("Dersin kredisi: "); scanf("%d",&kredi);
					ders_ekle(&head_ders,kod,ad,kapasite,kredi);
					printf("Yeni ders ekleme/silme islemi icin 1'e, ana menuye donmek icin 0'a basin: "); scanf("%d",&secim); while ((getchar()) != '\n');
				}
				else if(secim==2){
					printf("Lutfen dersin kodunu giriniz: "); fgets(kod, 10, stdin); 
					kod[strcspn(kod, "\n")] = 0;
					ders_cikar(&head_ders,kod,&head_kayit,gun,ay,yil);
					printf("Yeni ders ekleme/silme islemi icin 1'e, ana menuye donmek icin 0'a basin: "); scanf("%d",&secim); while ((getchar()) != '\n');
				}
			}
			kayit_yazdir(head_kayit); sinif_listeleri(head_ders); ders_programlari(head_ogrenci,head_ders);
			ogrenci_yazdir(head_ogrenci); ders_yazdir(head_ders);
		}
		else if(secim==2){
			secim=1;
			int no, ders_sayisi, toplam_kredi;
			char ad[30];
			while(secim==1){
				printf("Ogrenci eklemek icin 1'i, silmek icin 2'yi tuslayin: "); scanf("%d",&secim); while ((getchar()) != '\n');
				if(secim==1){
					printf("Adi Soyadi: "); fgets(ad, 30, stdin); ad[strcspn(ad, "\n")] = 0;
					printf("Numarasi: "); scanf("%d",&no);
					ogrenci_ekle(&head_ogrenci,no,ad);
					printf("Yeni ogrenci ekleme/silme islemi icin 1'e, ana menuye donmek icin 0'a basin: "); scanf("%d",&secim); while ((getchar()) != '\n');
				}
				else if(secim==2){
					printf("Numarasi: "); scanf("%d",&no);
					ogrenci_cikar(&head_ogrenci,no);
					printf("Yeni ogrenci ekleme/silme islemi icin 1'e, ana menuye donmek icin 0'a basin: "); scanf("%d",&secim); while ((getchar()) != '\n');
				}
			}
			kayit_yazdir(head_kayit); sinif_listeleri(head_ders); ders_programlari(head_ogrenci,head_ders);
			ogrenci_yazdir(head_ogrenci);  ders_yazdir(head_ders);
		}
		else if(secim==3){
			secim=1;
			char kod[10];
			int no;
			while(secim==1){
				printf("Derse kaydolmak icin 1'i, kayit silmek icin 2'yi tuslayin: "); scanf("%d",&secim); while ((getchar()) != '\n');
				if(secim==1){
					printf("Ders kodu: "); fgets(kod, 10, stdin); kod[strcspn(kod, "\n")] = 0;
					printf("Ogrenci numarasi: "); scanf("%d",&no);
					derse_kayit(&head_ders,no,kod,&head_kayit,gun,ay,yil,&head_ogrenci,max_ders,max_kredi);
					printf("Yeni kayit ekleme/silme islemi icin 1'e, ana menuye donmek icin 0'a basin: "); scanf("%d",&secim); while ((getchar()) != '\n');
				}
				else if(secim==2){
					printf("Ders kodu: "); fgets(kod, 10, stdin); kod[strcspn(kod, "\n")] = 0;
					printf("Ogrenci numarasi: "); scanf("%d",&no);
					dersi_birak(&head_ders,no,kod,&head_kayit,gun,ay,yil,&head_ogrenci);
					printf("Yeni kayit ekleme/silme islemi icin 1'e, ana menuye donmek icin 0'a basin: "); scanf("%d",&secim); while ((getchar()) != '\n');
				}
			}
			kayit_yazdir(head_kayit); sinif_listeleri(head_ders); ders_programlari(head_ogrenci,head_ders);
			ogrenci_yazdir(head_ogrenci);  ders_yazdir(head_ders);
		}
	}while(secim==0);
	
	while(head_ders->next!=NULL){
		free(head_ders->kod);
		free(head_ders->ad);
		free(head_ders->numaralar);
		free(head_ders);
		head_ders=head_ders->next;
	} 
	while(head_ogrenci->next!=NULL){
		free(head_ogrenci->ad);
		free(head_ogrenci);
		head_ogrenci=head_ogrenci->next;
	} 
	while(head_kayit->next!=NULL){
		free(head_kayit->kod);
		free(head_kayit->durum);
		free(head_kayit);
		head_kayit=head_kayit->next;
	}
	return 0;
}
void ogrenci_dosyasini_oku(OGRENCI **head){
	FILE *dosya;
	dosya=fopen("Ogrenciler.txt","r");
	int numara;
	char ad[30], soyad[15], tam_isim[30];
	while (fscanf(dosya,"%d",&numara)==1 && fscanf(dosya,"%s",ad)==1){
		ogrenci_ekle(head,numara,ad);
	}
	fclose(dosya);
}
void ders_dosyasini_oku(DERS **head){
	FILE *dosya;
	dosya=fopen("Dersler.txt","r");
	char kod[10], ad[40];
	int kapasite, kredi;
	while(fscanf(dosya,"%s",kod)==1 && fscanf(dosya,"%s",ad)==1 && fscanf(dosya,"%d",&kapasite)==1 && fscanf(dosya,"%d",&kredi)==1){
		ders_ekle(head,kod,ad,kapasite,kredi);
	}
	fclose(dosya);
}
void kayit_ekle(KAYIT **head,char kod[10],int no,int gun,int ay,int yil,char durum[15]){
	KAYIT *yeni=(KAYIT*)malloc(sizeof(KAYIT));
	yeni->kod=(char*)malloc(10*sizeof(char));
	yeni->durum=(char*)malloc(15*sizeof(char));
	int i=1;
	if(*head==NULL){ //deger yoksa
		*head=yeni;
		(*head)->next=NULL;
		yeni->next=NULL;
	}
	else{
		KAYIT *tmp=*head;
		i++;
		while(tmp->next!=NULL){
			tmp=tmp->next;
			i++;
		}
		tmp->next=yeni;
		yeni->next=NULL;
	}
	yeni->id=i;
	strcpy(yeni->kod,kod);
	yeni->no=no;
	yeni->gun=gun;
	yeni->ay=ay;
	yeni->yil=yil;
	strcpy(yeni->durum,durum);
}
void ogrenci_ekle(OGRENCI **head,int numara,char ad[30]){
	OGRENCI *yenio=(OGRENCI*)malloc(sizeof(OGRENCI));
	yenio->ad=(char*)malloc(30*sizeof(char));
	if(*head==NULL){ //deger yoksa
		*head=yenio;
		(*head)->next=NULL;
		yenio->next=NULL;
	}
	else if((*head)->next!=NULL && (numara<(*head)->next->numara && numara>(*head)->numara)){
		yenio->next=(*head)->next;
		(*head)->next->prev=yenio;
		(*head)->next=yenio;
		yenio->prev=*head;
	}
	else{
		OGRENCI *tmp=*head;
		if(numara<(tmp->numara)){ //yeni deger ilk degerden kucukse
			*head=yenio;
			yenio->next=tmp;
			tmp->prev=*head;
		}
		else{
			while(tmp->next!=NULL && numara>(tmp->numara)){ //tmp degerini yeni degerin bir oncesi yapalim
				tmp=tmp->next;
			}
			if(tmp->next==NULL){ //sona geldiyse (yeni deger en sona eklenir)
				tmp->next=yenio;
				yenio->next=NULL;
				yenio->prev=tmp;
			}
			else{ //araya eklenecekse
				tmp->prev->next=yenio;
				yenio->prev=tmp->prev;
				tmp->prev=yenio;
				yenio->next=tmp;
			}
		}
	}
	yenio->numara=numara;
	strcpy(yenio->ad,ad);
	yenio->ders_sayisi=0;
	yenio->toplam_kredi=0;
}
void derse_kayit(DERS **head,int no,char kod[10],KAYIT **head_kayit,int gun,int ay,int yil,OGRENCI **head_ogrenci,int max_ders,int max_kredi){
	DERS *tmp=*head;
	OGRENCI *ogr=*head_ogrenci;
	while(no!=ogr->numara){
		ogr=ogr->next;
	} 
	while(strcmp(tmp->kod,kod)!=0 && tmp->next!=NULL){
		tmp=tmp->next;
	}
	if(tmp->next==NULL && strcmp(tmp->kod,kod)!=0){
		printf("HATA! Boyle bir ders yoktur!\n");
	}
	else if(tmp->ogrenci_sayisi==tmp->kapasite){
		printf("Malesef bu dersin kapasitesi doludur :(\n");
	}
	else if(ogr->ders_sayisi >= max_ders){
		printf("Bu ogrenci maximum ders hakkini doldurmustur\n");
	}
	else if(ogr->toplam_kredi + tmp->kredi >= max_kredi){
		printf("Lutfen maximum kredi miktarini asmayacak sekilde ders seciniz\n");
	}
	else if(strcmp(tmp->kod,kod)==0){
		int flag=0, i;
		for(i=0;i<(tmp->ogrenci_sayisi);i++){ //ogrenci zaten dersi aliyor mu diye kontrol
			if(tmp->numaralar[i]==no){
				flag=1;
				printf("HATA! Bu ogrenci bu dersi zaten almaktadir\n");
			}
		}
		if(flag==0){
			if(tmp->ogrenci_sayisi==0){
				tmp->numaralar=(int*)malloc(sizeof(int));
				tmp->numaralar[0]=no;
				tmp->ogrenci_sayisi=1;
				ogr->ders_sayisi++;
				ogr->toplam_kredi+=tmp->kredi;
			}
			else{
				tmp->ogrenci_sayisi++;
				tmp->numaralar=(int*)realloc(tmp->numaralar,(tmp->ogrenci_sayisi)*sizeof(int));
				tmp->numaralar[tmp->ogrenci_sayisi - 1]=no;
				ogr->ders_sayisi++;
				ogr->toplam_kredi+=tmp->kredi;
			}
			kayit_ekle(head_kayit,kod,no,gun,ay,yil,"KAYITLI"); 
		}
	}
}
void dersi_birak(DERS **head,int no,char kod[10],KAYIT **head_kayit,int gun,int ay,int yil,OGRENCI **head_ogrenci){
	DERS *tmp=*head;
	OGRENCI *ogr=*head_ogrenci;
	while(no!=ogr->numara){
		ogr=ogr->next;
	}
	while(strcmp(tmp->kod,kod)!=0 && tmp->next!=NULL){
		tmp=tmp->next;
	}
	if(tmp->next==NULL && strcmp(tmp->kod,kod)!=0){
		printf("HATA! Boyle bir ders yoktur!");
	}
	else if(strcmp(tmp->kod,kod)==0){
		int i=0;
		while(i<(tmp->ogrenci_sayisi) && tmp->numaralar[i]!=no){
			i++;
		}
		if(i==tmp->ogrenci_sayisi){
			printf("HATA! Bu ogrenci bu dersi almamaktadir!\n");
		}
		else{
			while(i<(tmp->ogrenci_sayisi - 1)){
				tmp->numaralar[i]=tmp->numaralar[i+1];
				i++;
			}
			tmp->ogrenci_sayisi--;
			ogr->ders_sayisi--;
			ogr->toplam_kredi-=tmp->kredi;
			kayit_ekle(head_kayit,kod,no,gun,ay,yil,"BIRAKTI");
		}
	}
}
void ders_ekle(DERS **head,char kod[10],char ad[40],int kapasite,int kredi){
	int flag=0;
	DERS *yenid=(DERS*)malloc(sizeof(DERS));
	yenid->kod=(char*)malloc(10*sizeof(char));
	yenid->ad=(char*)malloc(40*sizeof(char)); 
	if(*head==NULL){ //deger yoksa
		*head=yenid;
		(*head)->next=NULL;
		yenid->next=NULL;
	}
	else{
		DERS *tmp=*head;
		if(strcmp(tmp->kod,kod)==1){ //yeni deger ilk degerden kucukse
		*head=yenid;
		yenid->next=tmp;
		}
		else{
			while(tmp->next!=NULL && strcmp(tmp->next->kod,kod) == -1 && strcmp(tmp->kod,kod) != 0){ //tmp degerini yeni degerin bir oncesi yapalim
				tmp=tmp->next;
			}
			if(strcmp(tmp->kod,kod)==0){ //ayni dersten var mi diye bakalim
				printf("HATA! Boyle bir ders zaten var!\n");
				flag=1;
			}
			else{
				if(tmp->next==NULL){ //sona geldiyse (yeni deger en sona eklenir)
					tmp->next=yenid;
					yenid->next=NULL;
				}
				else{ //araya eklenecekse
					yenid->next=tmp->next;
					tmp->next=yenid;
				}
			}
		}
	}
	if(flag==0){
	strcpy(yenid->kod,kod);
	strcpy(yenid->ad,ad);
	yenid->kapasite=kapasite;
	yenid->kredi=kredi;
	yenid->ogrenci_sayisi=0;
	}
}
void ogrenci_cikar(OGRENCI **head,int numara){
	OGRENCI *tmp=*head;
	if(numara==(*head)->numara){
		*head=(*head)->next;
		(*head)->next=tmp->next->next;
		(*head)->prev=NULL;
		(*head)->next->prev=*head;
	}
	else{
		while(numara!=tmp->next->numara && tmp->next!=NULL){
			tmp=tmp->next;
		}
		if(tmp->next==NULL && numara!=tmp->numara){
			printf("Boyle bir ogrenci zaten yoktur!!!\n");
		}
		else{
			tmp->next=tmp->next->next;
		}
	}
}
void ders_cikar(DERS **head,char kod[10],KAYIT **head_kayit,int gun,int ay,int yil){
	DERS *tmp=*head;
	int i;
	if(strcmp((*head)->kod,kod)==0){ // ilk degerse
		if((*head)->next==NULL){
			for(i=0;i<(*head)->ogrenci_sayisi;i++){
				kayit_ekle(head_kayit,kod,(*head)->numaralar[i],gun,ay,yil,"DERS_KAPANDI");
			}
			*head=NULL;
		}
		else{
			for(i=0;i<(*head)->ogrenci_sayisi;i++){
				kayit_ekle(head_kayit,kod,(*head)->numaralar[i],gun,ay,yil,"DERS_KAPANDI");
			}
			*head=(*head)->next;
			(*head)->next=tmp->next->next;
		}
	}
	else{
		while(strcmp(tmp->next->kod,kod)!=0 && tmp->next!=NULL){
			tmp=tmp->next;
		}
		if(tmp->next==NULL){
			printf("Boyle bir ders zaten yoktur!!!\n");
		}
		else{
			for(i=0;i<tmp->next->ogrenci_sayisi;i++){
				kayit_ekle(head_kayit,kod,tmp->next->numaralar[i],gun,ay,yil,"DERS_KAPANDI");
			}
			tmp->next=tmp->next->next;
		}
	}
}
void ders_yazdir(DERS *head){
	DERS *tmp=head;
	FILE *dosya;
	dosya=fopen("Dersler.txt","w");
	while(tmp!=NULL){
		fprintf(dosya,"%s %s %d %d\n",tmp->kod,tmp->ad,tmp->kapasite,tmp->kredi);
		tmp=tmp->next;
	}
	fclose(dosya);
}
void ogrenci_yazdir(OGRENCI *head){
	OGRENCI *tmp=head;
	FILE *dosya;
	dosya=fopen("Ogrenciler.txt","w");
	while(tmp!=NULL){
		fprintf(dosya,"%d %s\n",tmp->numara,tmp->ad);
		tmp=tmp->next;
	}
	fclose(dosya);
}
void kayit_yazdir(KAYIT *head){
	FILE *dosya;
	dosya=fopen("OgrenciDersKayit.txt","w");
	KAYIT *tmp=head;
	while(tmp!=NULL){
		fprintf(dosya,"%d %s %d %d.%d.%d %s\n",tmp->id,tmp->kod,tmp->no,tmp->gun,tmp->ay,tmp->yil,tmp->durum);
		tmp=tmp->next;
	}
	fclose(dosya);
}
void sinif_listeleri(DERS *head){
	int i;
	DERS *tmp=head;
	while(tmp!=NULL){
		FILE *dosya;
		char kod[14];
		strcpy(kod,tmp->kod);
		strncat(kod,".txt",4);
		dosya=fopen(kod,"w");
		for(i=0;i<tmp->ogrenci_sayisi;i++){
			fprintf(dosya,"%d\n",tmp->numaralar[i]);
		}
		fclose(dosya);
		tmp=tmp->next;
	}
}
void ders_programlari(OGRENCI *head,DERS *head_ders){
	OGRENCI *tmp=head;
	while(tmp!=NULL){
		FILE *dosya;
		char numara[30];
		sprintf(numara,"%d%s",tmp->numara,"_DERSPROGRAMI.txt");
		dosya=fopen(numara,"w");
		DERS *ders=head_ders;
		while(ders!=NULL){
			int i;
			for(i=0;i<ders->ogrenci_sayisi;i++){
				if((ders->numaralar[i])==(tmp->numara)){
					fprintf(dosya,"%s\n",ders->kod);
				}
			}
			ders=ders->next;
		}
		fclose(dosya);
		tmp=tmp->next;
	}
}
