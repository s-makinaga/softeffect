

int _EffColas( _WAVEHEADINFO *wavinfo, int *data, int *olddata ){
	
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
	
	
	//--------------ˆ—•”•ª‚±‚±‚©‚ç---------------
	
	
	memcpy( newData, data, 4*tagWaveinfo.DataNum );
	for( i=1323; i<wavinfo->DataNum; i++ ){
		newData[i] = (data[i] + data[i-1323])/2;
		
	}for( i=1323; i<wavinfo->DataNum; i++ ){
		newData[i] = (data[i] + data[i-1323])/2;
		
	}
	
	
	//---------------ˆ—•”•ª‚±‚±‚Ü‚Å----------------
	
	
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