#pragma once

#include <string>
#include <exception>

class BmSettings
{
public:
 	unsigned getBlockSize() const {
		return blockSize;
	}

 	unsigned getExtent() const {
		return extent;
	}

 	unsigned getNStep() const {
		return NStep;
	}

 	double getGammaP() const {
		return gammaP;
	}

 	double getGammaD() const {
		return gammaD;
	}

 	unsigned getNs() const {
		return Ns;
	}

 	double getGammaW() const {
		return gammaW;
	}

 	double getSigmaW() const {
		return sigmaW;
	}

 	double getTauTraj() const {
		return tauTraj;
	}

 	double getTauMatch() const {
		return tauMatch;
	}

 	unsigned getNg() const {
		return Ng;
	}

 	unsigned getM() const {
		return M;
	}

private:
	friend class Settings;
	BmSettings() {}

	// block (B(x0, t0)) size
	unsigned blockSize;
	// temporal extent, h;
	// we'll try to construct volumes (V(x0, t0)) at frames [t0-h, t0+h]
	unsigned extent;
	// step between reference blocks
	unsigned NStep;
	// velocity coefficient
	double gammaP;
	// block distance penaltization coefficient
	double gammaD;
	// block search window size
	unsigned Ns;
	double gammaW;
	double sigmaW;
	// block similarity threshold
	double tauTraj;
	// volumes similarity threshold
	double tauMatch;
	// volume search window size
	unsigned Ng;
	// Number of volumes in group
	unsigned M;
};

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

	const BmSettings& getHtBmSettings() const {
		return htBmSettings;
	}

	const BmSettings& getWieBmSettings() const {
		return wieBmSettings;
	}
	
private:
	std::string inputFile;
	std::string outputFile;
	// standard deviation of additive Gaussian noise
	unsigned sigma;

	// blockmatching settings
	BmSettings htBmSettings;
	BmSettings wieBmSettings;
};
