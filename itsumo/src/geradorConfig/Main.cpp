#include "leNet.h"

int main(int argc, char *argv[]){
	leNet ln;
	TiXmlDocument doc=ln.loadFile(argv[1]);
	vector<street> streets;
	simulation s;

	s=ln.leRede(doc);

/*
	if(argc==3){
		s=ln.leRede(doc);
		ln.setName(argv[1]);
		ln.setCaminho(argv[1]);
		ln.setDuracaoCiclo(atoi(argv[2]));
		ln.geraTrafficLight(s, doc);
	}else{
		cout<<endl<<"##########################"<<endl;
		cout<<"Falta de parametros:"<<endl;
		cout<<"./executavel netFile.xml duracaoCiclo"<<endl;
		cout<<endl<<"##########################"<<endl;
	}
*/
}
