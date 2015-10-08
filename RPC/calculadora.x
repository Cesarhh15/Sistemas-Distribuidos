struct flotantes{
	float a;
	float b;
};
struct flotantes_trigonometricas{
	float a;
};
program rpc{
	version rpcprueba{
		float suma(flotantes) = 1;
		float resta(flotantes) = 2;
		float multiplica(flotantes) = 3;
		float divide(flotantes) = 4;
		float seno(flotantes_trigonometricas)=5;
		float coseno(flotantes_trigonometricas)=6;
		float tangente(flotantes_trigonometricas)=7;

	} = 1;
}=0xffff;