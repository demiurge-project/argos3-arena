/**
 * @file <argos3/plugins/robots/arena/simulator/arena_entity.h>
 *
 * @author David Garzon-Ramos - <dgarzonr@ulb.ac.be>
 */

#ifndef ARENA_ENTITY_H
#define ARENA_ENTITY_H

#include <argos3/core/simulator/space/space.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/media/led_medium.h>
#include "block_entity.h"

namespace argos {
   class CBlockEntity;
   class CArenaEntity;
}

namespace argos {

   class CArenaEntity : public CComposableEntity {

   public:

       ENABLE_VTABLE();

      CArenaEntity();

      CArenaEntity(
                 const std::string& str_id,
                 const CVector3& c_position,
                 const CQuaternion& c_orientation,
                 const CVector3& c_size,
                 const std::string& str_led_medium,
                 UInt32 un_boxes,
                 UInt32 un_edges,
                 Real f_gap = 0.03,
                 Real f_mass = 1.0f);

      virtual void Init(TConfigurationNode& t_tree);
      virtual void Reset();
      virtual void Destroy();

      virtual std::string GetTypeDescription() const {
         return "arena";
      }

      void AddWall(std::vector<CBlockEntity*> c_wall);

      inline CPositionalEntity& GetPositionalEntity() {
         return *m_pcPositionalEntity;
      }

      inline const CPositionalEntity& GetPositionalEntity() const {
         return *m_pcPositionalEntity;
      }

      inline const std::vector<CBlockEntity*>& GetBlocks() {
         return m_vBlocks;
      }

      inline const std::vector<std::vector<CBlockEntity*>>& GetWalls() {
         return m_vWalls;
      }

      void PositionWalls();

      void SetArenaColor(CColor vColor);

      void SetWallColor(SInt32 unWallID, CColor vColor);

      void SetBoxColor(SInt32 unBoxID, SInt32 unWallID, CColor vColor);

      void AddBlock(CBlockEntity& c_block);

      void PositionBlocks(UInt32 c_wallId, CVector3 c_wallPosition, CQuaternion c_wallOrientation);

      // TODO

      void SetReferenceBlock(UInt32 unBoxID){
          m_unReferenceBlock = unBoxID;
      }

      UInt32 GetReferenceBlock(UInt32){
          return m_unReferenceBlock;
      }

      void SetBoxColor(SInt32 unBoxID, CColor vColor);

      void SetLedColor(SInt32 unledID, SInt32 unBoxID, SInt32 unWallID, CColor vColor);

      void SetLedColor(SInt32 unBoxID, SInt32 unWallID, CColor vColor);

      void SetLedColor(SInt32 unWallID, CColor vColor);

      void SetLedColor(CColor vColor);

      UInt32 GetBoxParent();

      //

      bool IsEven(UInt32 unNumber);

      Real InnerRadious();

   private:

      CPositionalEntity*        m_pcPositionalEntity;
      std::vector<CBlockEntity*>  m_vBlocks;
      std::vector<std::vector<CBlockEntity*>>  m_vWalls;
      std::string               m_strLEDMedium;
      CLEDMedium*               m_pcLEDMedium;
      CQuaternion               m_cOrientation;
      CVector3                  m_cPosition;
      CVector3                  m_cSize;
      Real                      m_fMass;
      Real                      m_fGap;
      UInt32                    m_unNumberBoxes;
      UInt32                    m_unNumberEdges;
      UInt32                    m_unReferenceBlock;

      //TODO

      CControllableEntity*        m_pcControllableEntity;

   };

}

#endif
