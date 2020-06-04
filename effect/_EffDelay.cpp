

int _EffDelay( _WAVEHEADINFO *wavinfo, int *data, int *olddata ){
	
	int level;
	int deeps;
	int kaisu;
	int tone;
	int i,j,x;
	int *newData;
	//int *ecoData;
	
	memcpy( olddata, data, 4*tagWaveinfo.DataNum );
	
	deeps = 20000; kaisu = 20; level=50; tone=50; 
	
	newData = (int *)malloc( wavinfo->DataNum*4 );
	for( i=0;i<wavinfo->DataNum;i++ ){
		newData[i]=0;
		data[i]-=128;
	}
	
	for( i=0; i<wavinfo->DataNum; i++ ){
		if( i<(deeps) ){
			newData[i] += data[i];
		}else{
			newData[i] += (data[i] + data[i-deeps]*(1))/2;
			for( j=0; j<kaisu; j++ ){
				if( (i-deeps-(500*j)) > 0 ){
					newData[i] = (data[i] + (int)(newData[i-deeps-(500*j)]*0.8))*0.8;
			}       }
			//newData[i]/=2;
		}
	}
	for( i=0; i<wavinfo->DataNum; i++ ){
		if( newData[i] < -128 ) newData[i] = -128;
		if( newData[i] > 128 ) newData[i] = 128;
		newData[i]+=128;
		data[i]+=128;
		//data[i] = newData[i];
	}
	memcpy( data, newData, 4*tagWaveinfo.DataNum );
	
	_SetWaveFile( "temp.wav", data, dWaveDataR, &tagWaveinfo );

	free( newData );
	
	return 0;
}