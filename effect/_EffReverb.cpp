

int _EffReverb( _WAVEHEADINFO *wavinfo, int *data, int *olddata ){
	
	int level;
	int deeps;
	int kaisu;
	int tone;
	int i,j,x;
	int *newData;
	//int *ecoData;
	
	memcpy( olddata, data, 4*tagWaveinfo.DataNum );
	
	newData = (int *)malloc( wavinfo->DataNum*4 );
	for( i=0;i<wavinfo->DataNum;i++ ){
		newData[i]=0;
		data[i]-=128;
	}
	
	//--------------ˆ—•”•ª‚±‚±‚©‚ç---------------
	
	memcpy( newData, data, 4*tagWaveinfo.DataNum );
	for( i=5000; i<wavinfo->DataNum; i++ ){
		newData[i] = (newData[i] + newData[i-5000]);
		newData[i] = (newData[i] + newData[i-4000]*0.6);
		newData[i] = (newData[i] + newData[i-3000]*0.7);
		newData[i] = (newData[i] + newData[i-2000]*0.4);
		newData[i] = (newData[i] + newData[i-1000]*0.5)/4;
			
	}
	
	//--------------ˆ—•”•ª‚±‚±‚Ü‚Å---------------
	
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