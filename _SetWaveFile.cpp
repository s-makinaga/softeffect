#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void _SetWaveFile( char* filename, int* dataL, int* dataR, _WAVEHEADINFO *wavinfo ){
	
	FILE *fp;
	char s1='f';
	char s2='a';
	char s3='c';
	char s4='t';
	int i,j;
	byte *byteWaveData;
	
	j=0;
	byteWaveData = (byte *)malloc( wavinfo->DataNum*2 );
	if( wavinfo->Channels == 1 ){
		for( i=0; i<wavinfo->DataNum; i++ ){
			byteWaveData[i]=(byte)dataL[i];
		}
	} else if( wavinfo->Channels == 2 ){
		for( i=0; i<wavinfo->DataNum*2; i+=2 ){
			byteWaveData[i]=(byte)dataL[j];
			byteWaveData[i+1]=(byte)dataR[j];
			j++;
		}
	}
	
	if((fp = fopen(filename, "wb")) == NULL ) {
		printf("ファイルオープンエラー\n");
		exit(1);
	}
	
	fseek( fp, 0, SEEK_SET );
	fwrite( &wavinfo->RiffHead, 4, 1, fp );
	fwrite( &wavinfo->RiffHeadSize, 4, 1, fp );
	fwrite( &wavinfo->WaveHead, 4, 1, fp );
	fwrite( "fmt ", 4, 1, fp );
	fwrite( &wavinfo->FileFormatSize, 4, 1, fp );
	fwrite( &wavinfo->FormatID, 2, 1, fp );
	fwrite( &wavinfo->Channels, 2, 1, fp );
	fwrite( &wavinfo->Samples, 4, 1, fp );
	fwrite( &wavinfo->DataSpeed, 4, 1, fp );
	fwrite( &wavinfo->BlockSize, 2, 1, fp );
	fwrite( &wavinfo->SampleBit, 2, 1, fp );
  	
	fwrite( "aa", 2, 1, fp );
	
	fwrite( "fact", 4, 1, fp );
	
	fwrite( &wavinfo->FactChankSize, 4, 1, fp );
	fwrite( &wavinfo->AllData, 4, 1, fp );
	fwrite( "data", 4, 1, fp );
	fwrite( &wavinfo->DataSize, 4, 1, fp );
	
	fseek( fp, wavinfo->Offset, SEEK_SET );
	
	
	
	if( wavinfo->Channels == 1 ){
		fwrite( byteWaveData, 1, wavinfo->DataNum, fp );
	} else if( wavinfo->Channels == 2 ){
		fwrite( byteWaveData, 1, wavinfo->DataNum*2, fp );
	}
	
	fclose( fp );
}

