/*============================================================================
Copyright 2017-2021 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef MEDICAL_BEAM_H
#define MEDICAL_BEAM_H

#include "G4VUserPrimaryGeneratorAction.hh"

class G4ParticleDefinition;

class MedicalBeam : public G4VUserPrimaryGeneratorAction {
public:
  MedicalBeam();
  ~MedicalBeam() = default;

  MedicalBeam(const MedicalBeam&) = delete;
  void operator=(const MedicalBeam&) = delete;

  enum { kElectron = 0, kPhoton, kProton};

  void SetParticle(int ptype);
  int GetParticle() const;

  void SetEnergy(double energy);
  double GetEnergy() const;

  // distruubuted energy spectrum generated by specified MV electron (6/18 MV)
  void SetPhotonVoltage(int volt);
  int GetPhotonVoltage() const;

  void SetSSD(double val_ssd);
  double GetSSD() const;

  void SetFieldSize(double val_xy);
  double GetFieldSize() const;

  virtual void GeneratePrimaries(G4Event* event);

private:
  int particle_type_;
  double kinetic_energy_;
  int photon_voltage_;

  double ssd_;
  double field_xy_;
};

// ====================================================================
inline void MedicalBeam::SetParticle(int ptype)
{
  particle_type_ = ptype;
}

inline int MedicalBeam::GetParticle() const
{
  return particle_type_;
}

inline void MedicalBeam::SetEnergy(double energy)
{
  kinetic_energy_ = energy;
}

inline double MedicalBeam::GetEnergy() const
{
  return kinetic_energy_;
}

inline int MedicalBeam::GetPhotonVoltage() const
{
  return photon_voltage_;
}

inline void MedicalBeam::SetSSD(double val_ssd)
{
  ssd_ = val_ssd;
}

inline double MedicalBeam::GetSSD() const
{
  return ssd_;
}

inline void MedicalBeam::SetFieldSize(double val_xy)
{
  field_xy_ = val_xy;
}

inline double MedicalBeam::GetFieldSize() const
{
  return field_xy_;
}

#endif
