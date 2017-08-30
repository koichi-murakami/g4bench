/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include <algorithm>
#include <cmath>
#include <vector>
#include "G4Event.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4PrimaryVertex.hh"
#include "G4Proton.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4ThreeVector.hh"
#include "CLHEP/Random/RandFlat.h"
#include "medicalbeam.h"

namespace {

// photon voltage
enum { k6MV = 6, k18MV = 18 };

// --------------------------------------------------------------------------
G4ThreeVector GenerateBeamDirection(double ssd, double fxy)
{
  const double dr = std::sqrt(2.) * fxy / 2.;
  const double R = std::sqrt(sqr(ssd)+sqr(dr));
  const double cos0 = ssd/R;

  const double xymax = fxy/ssd/2.;
  double dx, dy, dz, dsin, dphi;

  do {
    dz = CLHEP::RandFlat::shoot(cos0, 1.);
    dsin = std::sqrt(1.-sqr(dz));
    dphi = CLHEP::RandFlat::shoot(0., twopi);

    dx = dsin * std::cos(dphi);
    dy = dsin * std::sin(dphi);
  } while(! (std::abs(dx) < xymax*dz && std::abs(dy) < xymax*dz) );

  return G4ThreeVector(dx,dy,dz);
}

// --------------------------------------------------------------------------
  const double kEbin = 250.*keV;

  // 6 MV
  enum { kSize6 = 29 };
  const double vec_rprob6[kSize6] = {
    0.,
    9.4485E-06, 9.5786E-06, 8.0766E-06, 6.8650E-06, 5.8836E-06, 5.0212E-06,
    4.2913E-06, 3.7020E-06, 3.2493E-06, 2.7831E-06, 2.4103E-06, 2.1283E-06,
    1.8404E-06, 1.5743E-06, 1.4005E-06, 1.2077E-06, 1.0442E-06, 8.6051E-07,
    7.4537E-07, 5.7857E-07, 4.3973E-07, 2.8741E-07, 1.2629E-07, 7.6060E-09,
    7.6060E-19, 7.6060E-19, 7.6060E-19, 7.6060E-24
  };

  // 18 MV
  enum { kSize18 = 76 };
  const double vec_rprob18[kSize18] = {
    0.,
    4.0337E-06, 9.5959E-06, 1.2065E-05, 1.3781E-05, 1.4319E-05, 1.4062E-05,
    1.3924E-05, 1.3086E-05, 1.2041E-05, 1.1453E-05, 1.0889E-05, 1.0112E-05,
    9.3359E-06, 8.7616E-06, 8.1366E-06, 7.6597E-06, 7.1010E-06, 6.7447E-06,
    6.2360E-06, 5.8474E-06, 5.4393E-06, 5.2179E-06, 4.8653E-06, 4.6058E-06,
    4.2906E-06, 4.0800E-06, 3.8501E-06, 3.6913E-06, 3.6273E-06, 3.2497E-06,
    3.1424E-06, 2.8892E-06, 2.8118E-06, 2.6369E-06, 2.5523E-06, 2.3753E-06,
    2.2205E-06, 2.1544E-06, 2.0812E-06, 2.0213E-06, 1.9223E-06, 1.7353E-06,
    1.7610E-06, 1.6670E-06, 1.5800E-06, 1.4863E-06, 1.3896E-06, 1.4569E-06,
    1.2893E-06, 1.2828E-06, 1.1637E-06, 1.0773E-06, 1.0395E-06, 1.0556E-06,
    9.6263E-07, 9.1664E-07, 8.9720E-07, 8.7556E-07, 7.7567E-07, 7.4345E-07,
    6.8121E-07, 6.3706E-07, 5.4163E-07, 5.6762E-07, 4.8476E-07, 4.3953E-07,
    4.0895E-07, 3.2791E-07, 2.8358E-07, 2.1152E-07, 1.3752E-07, 6.5072E-08,
    1.3752E-07, 6.5072E-08, 6.5072E-13
  };

  std::vector<double> vec_energy6(kSize6), vec_cprob6(kSize6);
  std::vector<double> vec_energy18(kSize18), vec_cprob18(kSize18);

// --------------------------------------------------------------------------
void InitializePhotnSpectrum()
{
  for ( int i = 0; i < kSize6; i++ ) {
    vec_energy6[i] = kEbin * i;
    if ( i == 0 ) vec_cprob6[i] = 0.;
    else vec_cprob6[i] = vec_cprob6[i-1] + vec_rprob6[i];
  }

  for ( int i = 0; i < kSize18; i++ ) {
    vec_energy18[i] = kEbin * i;
    if ( i == 0 ) vec_cprob18[i] = 0.;
    else vec_cprob18[i] = vec_cprob18[i-1] + vec_rprob18[i];
  }
}

// --------------------------------------------------------------------------
double GeneratePhotonEnergy6MV()
{
  double rand_max = vec_cprob6[kSize6-1];
  double psampled = CLHEP::RandFlat::shoot(0., rand_max);
  std::vector<double>::iterator low =
    std::lower_bound( vec_cprob6.begin(), vec_cprob6.end(), psampled );

  size_t idx = low - vec_cprob6.begin();
  double rpos = ( psampled - vec_cprob6[idx-1] ) /
                ( vec_cprob6[idx] - vec_cprob6[idx-1] );
  double egen = vec_energy6[idx-1] + kEbin * rpos;

  return egen;
}

// --------------------------------------------------------------------------
double GeneratePhotonEnergy18MV()
{
  double rand_max = vec_cprob18[kSize18-1];
  double psampled = CLHEP::RandFlat::shoot(0., rand_max);
  std::vector<double>::iterator low =
    std::lower_bound( vec_cprob18.begin(), vec_cprob18.end(), psampled );

  size_t idx = low - vec_cprob18.begin();
  double rpos = ( psampled - vec_cprob18[idx-1] ) /
                ( vec_cprob18[idx] - vec_cprob18[idx-1] );
  double egen = vec_energy18[idx-1] + kEbin * rpos;

  return egen;
}

} // end of namespace

// --------------------------------------------------------------------------
MedicalBeam::MedicalBeam()
  : particle_type_(kElectron), kinetic_energy_(20.*MeV),
    photon_voltage_(::k6MV), ssd_(100.*cm), field_xy_(10.*cm)
{
  ::InitializePhotnSpectrum();
}

// --------------------------------------------------------------------------
MedicalBeam::~MedicalBeam()
{
}

// --------------------------------------------------------------------------
void MedicalBeam::SetPhotonVoltage(int volt)
{
  if ( volt != ::k6MV && volt != ::k18MV ) {
      std::cout << "[ ERROR ] MedicalBeam::SetPhotonVoltate() "
                   "photon voltage should be 6 or 18 MV, "
                << volt << std::endl;
      std::exit(EXIT_FAILURE);
  }

  photon_voltage_ = volt;
}

// --------------------------------------------------------------------------
void MedicalBeam::GeneratePrimaries(G4Event* event)
{
  G4ParticleDefinition* particle = nullptr;
  switch ( particle_type_ ) {
    case kElectron :
      particle = G4Electron::Electron();
      break;
    case kProton :
      particle = G4Proton::Proton();
      break;
    case kPhoton :
      particle = G4Gamma::Gamma();
      break;
    default :
      particle = G4Electron::Electron();
      break;
  }

  G4ThreeVector pvec;
  if ( particle_type_ == kPhoton ) {
    double energy;
    if ( photon_voltage_ == ::k6MV )
      energy = ::GeneratePhotonEnergy6MV();
    else if ( photon_voltage_ == ::k18MV )
      energy = ::GeneratePhotonEnergy18MV();
    else {
      std::cout << "[ ERROR ] MedicalBeam::GeneratePrimaries() "
                   "phton voltage should be 6 or 8 MV, "
                << photon_voltage_ << std::endl;
      std::exit(EXIT_FAILURE);
    }
    pvec = energy * ::GenerateBeamDirection(ssd_, field_xy_);
  } else {
    G4double mass = particle-> GetPDGMass();
    G4double momemtum = std::sqrt(sqr(mass+kinetic_energy_) - sqr(mass));
    pvec = momemtum * ::GenerateBeamDirection(ssd_, field_xy_);
  }

  G4PrimaryParticle* primary =
    new G4PrimaryParticle( particle, pvec.x(), pvec.y(), pvec.z() );

  const double kZoffset = 35. * cm;
  G4ThreeVector pos = G4ThreeVector(0., 0., kZoffset - ssd_);
  G4PrimaryVertex* vertex= new G4PrimaryVertex(pos, 0.*ns);
  vertex-> SetPrimary(primary);

  event-> AddPrimaryVertex(vertex);
}
