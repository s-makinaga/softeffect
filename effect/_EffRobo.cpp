

int _EffRobo( _WAVEHEADINFO *wavinfo, int *data, int *olddata ){
	
	int robo;
	int i,j,x;
	int *newData;
	//int *ecoData;
	
	memcpy( olddata, data, 4*tagWaveinfo.DataNum );
	
	robo = 3; 
	
	newData = (int *)malloc( wavinfo->DataNum*4 );
	for( i=0;i<wavinfo->DataNum;i++ ){
		newData[i]=0;
		data[i]-=128;
	}
	
	//--------------ˆ—•”•ª‚±‚±‚©‚ç---------------
	
	memcpy( newData, data, 4*tagWaveinfo.DataNum );
	for( j=0; j<=robo; j++ ){
	for( i=100; i<wavinfo->DataNum; i++ ){
		newData[i] = (newData[i] + newData[i-100])/2;
		
	}}
	
	//--------------ˆ—•”•ª‚±‚±‚Ü‚Å---------------
	
	for( i=0; i<wavinfo->DataNum; i++ ){
		newData[i]+=128;
		data[i]+=128;
		//data[i] = newData[i];
	}
	memcpy( data, newData, 4*tagWaveinfo.DataNum );
	
	_SetWaveFile( "temp.wav", data, dWaveDataR, &tagWaveinfo );

	free( newData );
	
	return 0;
}