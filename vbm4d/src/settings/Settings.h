#pragma once

#include <string>
#include <exception>

class Settings
{
public:
	class BadArgs : public std::exception {
	public:
		BadArgs(const std::string& msg) noexcept;
		virtual ~BadArgs() = default;
		virtual const char* what() const noexcept;
	private:
		std::string msg;
	};


	Settings(int argc, char** argv); //throw(BadArgs);

	std::string getInputFile() const {
		return inputFile;
	}

	std::string getOutputFile() const {
		return outputFile;
	}

	unsigned getSigma() const {
		return sigma;
	}
	
	unsigned getHtBlockSize() const {
		return htBlockSize;
	}

	unsigned getWieBlockSize() const {
		return wieBlockSize;
	}

	unsigned getHtExtent() const {
		return htExtent;
	}

	unsigned getWieExtent() const {
		return wieExtent;
	}

	unsigned getHtNStep() const {
		return htNStep;
	}

	unsigned getWieNStep() const {
		return wieNStep;
	}

	double getHtGammaP() const {
		return htGammaP;
	}

	double getWieGammaP() const {
		return wieGammaP;
	}

	double getHtGammaD() const {
		return htGammaD;
	}

	double getWieGammaD() const {
		return wieGammaD;
	}

	unsigned getHtNs() const {
		return htNs;
	}

	unsigned getWieNs() const {
		return wieNs;
	}

	double getHtGammaW() const {
		return htGammaW;
	}

	double getWieGammaW() const {
		return wieGammaW;
	}

	double getHtSigmaW() const {
		return htSigmaW;
	}

	double getWieSigmaW() const {
		return wieSigmaW;
	}

	double getHtTauTraj() const {
		return htTauTraj;
	}

	double getWieTauTraj() const {
		return wieTauTraj;
	}

	double getHtTauMatch() const {
		return htTauMatch;
	}

	double getWieTauMatch() const {
		return wieTauMatch;
	}

	unsigned getHtNg() const {
		return htNg;
	}

	unsigned getWieNg() const {
		return wieNg;
	}

	unsigned getHtM() const {
		return htM;
	}

	unsigned getWieM() const {
		return wieM;
	}

private:
// parameters set by a user
	std::string inputFile;
	std::string outputFile;
	// standard deviation of additive Gaussian noise
	unsigned sigma;

// parameters with default values
	// block (B(x0, t0)) size
	unsigned htBlockSize = 8;
	unsigned wieBlockSize = 7;
	// temporal extent, h;
	// we'll try to construct volumes (V(x0, t0)) at frames [t0-h, t0+h]
	unsigned htExtent = 4;
	unsigned wieExtent = 4;
	// step between reference blocks
	unsigned htNStep = 6;
	unsigned wieNStep = 4;
	// velocity coefficient
	double htGammaP = 0.3;
	double wieGammaP = 0.3;
	// block distance penaltization coefficient
	double htGammaD;
	double wieGammaD = 0.005;
	// block search window size
	unsigned htNs = 11;
	unsigned wieNs = 11;
	double htGammaW = 0.5;
	double wieGammaW = 0.5;
	double htSigmaW = 1.0;
	double wieSigmaW = 1.0;
	// block similarity threshold
	double htTauTraj;
	double wieTauTraj = 1.0;
	// volumes similarity threshold
	double htTauMatch;
	double wieHauMatch = 13.5;
	// volume search window size
	unsigned htNg = 19;
	unsigned wieNg = 27;
	// Number of volumes in group
	unsigned htM = 32;
	unsigned vieM = 8;
};
