int _EffDist( _WAVEHEADINFO *wavinfo, int *data, int *olddata ){
	
	int gain;
	int i,x;
	int *newData;
	
	gain = 50;
	newData = (int *)malloc( wavinfo->DataNum*4 );
	
	memcpy( olddata, data, 4*tagWaveinfo.DataNum );
	
	for( i=0; i<wavinfo->DataNum; i++ ) data[i] -=128;
	
	for( i=0; i<wavinfo->DataNum; i++ ){
		if( data[i] < 0 ){
			newData[i] = data[i] - gain;
		} else {
			newData[i] = data[i] + gain;
		}
	}
	
	for( i=0; i<wavinfo->DataNum; i++ ){
		if( newData[i] < -128 ) newData[i] = -128;
		if( newData[i] > 128 ) newData[i] = 128;
		newData[i] +=128;
		data[i] += 128;
	}
	
	memcpy( data, newData, 4*tagWaveinfo.DataNum );
	
	_SetWaveFile( "temp.wav", data, dWaveDataR, &tagWaveinfo );

	free( newData );
	
	return 0;
}