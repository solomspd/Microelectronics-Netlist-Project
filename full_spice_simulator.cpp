#include <iostream>
#include <string>
#include <fstream>
#include <vector>

using namespace std;


struct nmos {
	string type = "";
	string Rg = "3";
	string Vto = "1.4";
	string Rd = "8m";
	string Rs = "6m";
	string Rb = "10m";
	string Kp = "70";
	string Cgdmax = ".5n";
	string Cgdmin = ".12n";
	string Cgs = ".8n";
	string Cjo = ".24n";
	string Is = "24p";
	string ksubthres = ".1";
	string mfg = "Siliconix";
	string Vds = "12";
	string Ron = "20m";
	string Qg = "12n";
	
};

struct pmos {
	string type = "pchan";
	string Rg = "3";
	string Vto = "-1.3";
	string Rd = "16m";
	string Rs = "12m";
	string Rb = "20m";
	string Kp = "30";
	string Cgdmax = ".6n";
	string Cgdmin = ".14n";
	string Cgs = ".9n";
	string Cjo = ".28n";
	string Is = "28p";
	string ksubthres = ".1";
	string mfg = "Siliconix";
	string Vds = "-12";
	string Ron = "40m";
	string Qg = "14n";

};

struct pulse {

	int On_Vdd = 5;
	int Off_Vdd = 0;

	int Period;
	int half_Period;

};

struct voltageSource {

	int TransGate;
	const int gnd = 0;
	pulse PWL;
	
};



void voltageSources(string exp, int on = 5, int off = 0) {
	
	vector <voltageSource> Vdds;
	vector <char> var;
	
	
	for (int i = 0; i < exp.length(); i++) {
		if (isalpha(exp[i])) { 
		var.push_back(exp[i]); }
	}

	Vdds.resize(var.size());

	int maximumPeriod = pow(2, var.size());

	for (int i = 0; i < var.size(); i++) {



		Vdds[i].TransGate = int( var[i]);
				
		Vdds[i].PWL.Period = maximumPeriod ;
		Vdds[i].PWL.half_Period = maximumPeriod / 2;

		maximumPeriod = maximumPeriod / 2;

		Vdds[i].PWL.On_Vdd = on;
		Vdds[i].PWL.Off_Vdd = off;
	
		
	}


	for (int i = 0; i < var.size(); i++) {

		cout << 'V' << Vdds[i].TransGate << ' '
			<< Vdds[i].TransGate << ' ' << Vdds[i].gnd << ' ' << "pulse("
			<< Vdds[i].PWL.Off_Vdd << ' ' << Vdds[i].PWL.On_Vdd << ' '
			<< '0' << " 1n " << "1n " << Vdds[i].PWL.Period << 'm' << ' '
			<< Vdds[i].PWL.half_Period << 'm' << " )" << endl;
	}


}

void simulation(int max, char unit) {

	cout << ".tran " << max << unit << endl;

	cout << ".backanno" << endl << ".end" << endl;

}

void printCMOS(string exp) {

	pmos mypmos;
	nmos mynmos;

	cout << "*this is the generated netlist for the expression " << exp << endl;

	cout << "*models discription" << endl;


	voltageSources(exp);


	//nmos
	cout << ".model mynmos VDMOS(" << mynmos.type << " Rg=" << mynmos.Rg << " Vto=" << mynmos.Vto << " Rd=" << mynmos.Rd << endl << "+ Rs=" << mynmos.Rs
		<< " Rb=" << mynmos.Rb << " Kp=" << mynmos.Kp << " Cgdmax=" << mynmos.Cgdmax << " Cgdmin=" << mynmos.Cgdmin << endl << '+' <<
		" Cgs=" << mynmos.Cgs << " Cjo=" << mynmos.Cjo << " Is=" << mynmos.Is << " ksubthres=" << mynmos.ksubthres << endl << '+' <<
		" mfg=" << mynmos.mfg << " Vds=" << mynmos.Vds << " Ron=" << mynmos.Ron << " Qg=" << mynmos.Qg << endl << endl;

	cout << ".model mypmos VDMOS(" << mypmos.type << " Rg=" << mypmos.Rg << " Vto=" << mypmos.Vto << " Rd=" << mypmos.Rd << endl << "+ Rs=" << mypmos.Rs
		<< " Rb=" << mypmos.Rb << " Kp=" << mypmos.Kp << " Cgdmax=" << mypmos.Cgdmax << " Cgdmin=" << mypmos.Cgdmin << endl << '+' <<
		" Cgs=" << mypmos.Cgs << " Cjo=" << mypmos.Cjo << " Is=" << mypmos.Is << " ksubthres=" << mypmos.ksubthres << endl << '+' <<
		" mfg=" << mypmos.mfg << " Vds=" << mypmos.Vds << " Ron=" << mypmos.Ron << " Qg=" << mypmos.Qg << endl << endl;

	simulation(8, 'm');

}





int main() {

	
	printCMOS("a%b^c");
	




	system("pause");
	return 0;
}