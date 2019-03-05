/**
 * @file <argos3/plugins/robots/arena/simulator/arena_entity.cpp>
 *
 * @author David Garzon-Ramos - <dgarzonr@ulb.ac.be>
 */

#include "arena_entity.h"

namespace argos {


   /****************************************/
   /****************************************/

   CArenaEntity::CArenaEntity():
       CComposableEntity(NULL),
       m_pcPositionalEntity(NULL),
       m_pcLEDMedium(NULL){}

   CArenaEntity::CArenaEntity(const std::string& str_id,
                              const CVector3& c_position,
                              const CQuaternion& c_orientation,
                              const CVector3& c_size,
                              const std::string& str_led_medium,
                              UInt32 un_boxes,
                              UInt32 un_edges,
                              Real f_gap,
                              Real f_mass) :
      CComposableEntity(NULL, str_id){
      try {
         /*
          * Create and init components
          */
         /* Posicional entity */

         m_pcPositionalEntity = new CPositionalEntity(this, "pose_0", c_position, c_orientation);

         m_pcLEDMedium = &CSimulator::GetInstance().GetMedium<CLEDMedium>(str_led_medium);
         m_cSize = c_size;
         m_fMass = f_mass;
         m_fGap = f_gap;
         m_unNumberBoxes = un_boxes;
         m_unNumberEdges = un_edges;

         AddComponent(*m_pcPositionalEntity);

         PositionWalls();

         /* Update components */
         UpdateComponents();

      }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }

   /****************************************/
   /****************************************/

   void CArenaEntity::Init(TConfigurationNode& t_tree) {
       try {

           CComposableEntity::Init(t_tree);

           /* Parse XML to get the position of the arena */
           GetNodeAttribute(t_tree, "position", m_cPosition);

           /* Parse XML to get the orientation of the arena */
           GetNodeAttribute(t_tree, "orientation", m_cOrientation);

           /* Parse XML to get the medium for the LEDS in the arena */
           GetNodeAttribute(t_tree, "led_medium", m_strLEDMedium);

           /* Parse XML to get the size of each block in the arena */
           GetNodeAttribute(t_tree, "block_size", m_cSize);

           /* Parse XML to get the mass of each block in the arena */
           GetNodeAttribute(t_tree, "block_mass", m_fMass);

           /* Parse XML to get the spacing between LEDs in the blocks of the arena */
           GetNodeAttribute(t_tree, "led_spacing", m_fGap);

           /* Parse XML to get the number of blocks on each edge of the arena */
           GetNodeAttribute(t_tree, "n_blocks", m_unNumberBoxes);

           /* Parse XML to get the number of edges of of the arena */
           GetNodeAttribute(t_tree, "n_edges", m_unNumberEdges);

           m_pcLEDMedium = &CSimulator::GetInstance().GetMedium<CLEDMedium>(m_strLEDMedium);
           m_pcPositionalEntity = new CPositionalEntity(this, "pose_0", m_cPosition, m_cOrientation);
           AddComponent(*m_pcPositionalEntity);

           PositionWalls();

           /* Update components */
           UpdateComponents();

       }
      catch(CARGoSException& ex) {
         THROW_ARGOSEXCEPTION_NESTED("Failed to initialize entity \"" << GetId() << "\".", ex);
      }
   }


   /****************************************/
   /****************************************/

   void CArenaEntity::Reset() {
      /* Reset all components */
      CComposableEntity::Reset();
      /* Update components */
      UpdateComponents();
   }

   /****************************************/
   /****************************************/

   void CArenaEntity::Destroy() {

       CComposableEntity::Destroy();
   }

   /****************************************/
   /****************************************/

   void CArenaEntity::AddWall(CWallEntity& c_wall) {
      AddComponent(c_wall);
      m_vWalls.push_back(&c_wall);
      c_wall.Update();
   }

   /****************************************/
   /****************************************/

   void CArenaEntity::PositionWalls() {
     CWallEntity* pcWall;
     CRadians fAngle = (2 * CRadians::PI) / m_unNumberEdges;
     CRadians fAngleZ, fAngleY, fAngleX;
     m_pcPositionalEntity->GetOrientation().ToEulerAngles(fAngleZ,fAngleY,fAngleX);
     Real fRadious = InnerRadious();
     for(UInt32 i = 0; i < m_unNumberEdges; ++i) {
       std::ostringstream id;

       id << this->GetId() << ".wall_" << i;
       pcWall = new CWallEntity(this,
                                id.str().c_str(),
                                m_pcPositionalEntity->GetPosition() + CVector3((fRadious * Cos(fAngleZ + (fAngle * i))),
                                                                               (fRadious * Sin(fAngleZ + (fAngle * i))),
                                                                               0),
                                CQuaternion().FromEulerAngles(-CRadians::PI +(fAngleZ + (fAngle * i)),CRadians::ZERO,CRadians::ZERO),
                                m_cSize,
                                "leds",
                                m_unNumberBoxes,
                                m_fGap,
                                m_fMass);
       AddWall(*pcWall);
     }
   }

   /****************************************/
   /****************************************/

   void CArenaEntity::SetArenaColor(CColor vColor){
       CArenaEntity* pcArena = this;
       CWallEntity* pcWall;
       CBlockEntity* pcBlock;
       std::vector<CWallEntity*> pcWalls = pcArena->GetWalls();

       for (std::vector<CWallEntity*>::iterator itW = pcWalls.begin(); itW!=pcWalls.end(); ++itW){
           pcWall = m_vWalls.at(std::distance(pcWalls.begin(),itW));
           std::vector<CBlockEntity*> pcBlocks = pcWall->GetBlocks();
           for (std::vector<CBlockEntity*>::iterator itB = pcBlocks.begin(); itB!=pcBlocks.end(); ++itB){
               pcBlock = pcBlocks.at(std::distance(pcBlocks.begin(),itB));
               CLEDEquippedEntity& pcLED = pcBlock->GetLEDEquippedEntity();
               pcLED.SetAllLEDsColors(vColor);
               pcLED.Update();
           }
       }
   }

   /****************************************/
   /****************************************/

   void CArenaEntity::SetWallColor(SInt32 unWallID, CColor vColor){
       CWallEntity* pcWall;
       CBlockEntity* pcBlock;
       pcWall = m_vWalls.at(unWallID-1);
       std::vector<CBlockEntity*> pcBlocks = pcWall->GetBlocks();
       for (std::vector<CBlockEntity*>::iterator it = pcBlocks.begin(); it!=pcBlocks.end(); ++it){
           pcBlock = pcBlocks.at(std::distance(pcBlocks.begin(),it));
           CLEDEquippedEntity& pcLED = pcBlock->GetLEDEquippedEntity();
           pcLED.SetAllLEDsColors(vColor);
           pcLED.Update();
       }
   }

   /****************************************/
   /****************************************/

   void CArenaEntity::SetBoxColor(SInt32 unBoxID, SInt32 unWallID, CColor vColor){

       CWallEntity* pcWall;
       pcWall = m_vWalls.at(unWallID-1);
       CBlockEntity* pcBlock;
       pcBlock = pcWall->GetBlocks().at(unBoxID-1);
       CLEDEquippedEntity& pcLED = pcBlock->GetLEDEquippedEntity();
       pcLED.SetAllLEDsColors(vColor);
       pcLED.Update();
   }

   /****************************************/
   /****************************************/
/*
   void CArenaEntity::SetBoxColor(SInt32 unBoxID, CColor vColor){

       CWallEntity* pcWall;
       pcWall = m_vWalls.at(unWallID-1);
       CBlockEntity* pcBlock;
       pcBlock = pcWall->GetBlocks().at(unBoxID-1);
       CLEDEquippedEntity& pcLED = pcBlock->GetLEDEquippedEntity();
       pcLED.SetAllLEDsColors(vColor);
       pcLED.Update();
   }*/

   /****************************************/
   /****************************************/

   bool CArenaEntity::IsEven(UInt32 unNumber) {
       bool even;
       if((unNumber%2)==0)
          even = true;
       else
          even = false;

       return even;
   }

   /****************************************/
   /****************************************/

   Real CArenaEntity::InnerRadious() {
       Real fInnerRadious = (m_cSize.GetY() * m_unNumberBoxes) / (2 * Tan(CRadians::PI / m_unNumberEdges));
       return fInnerRadious;
   }

   /****************************************/
   /****************************************/

         REGISTER_ENTITY(CArenaEntity,
                         "cparena",
                         "David Garzon Ramos [dgarzonr@ulb.ac.be]",
                         "1.0",
                         "Convex polygonal arena, developed at IRIDIA - ULB.",
                         "The cparena is an arena designed to conduct experiments with the e-puck robot.\n"
                         "For more information, refer to the dedicated web page\n"
                         "(https://github.com/demiurge-project/argos3-arena).\n\n"
                         "REQUIRED XML CONFIGURATION\n\n"
                         "  <arena ...>\n"
                         "    ...\n"
                         "    <cparena id=\"cparena0\" position=\"0.0,0.0,0.0\" orientation=\"0,0,0\" block_size=\"0.01,0.25,0.1\" block_mass=\"1.0\" led_spacing=\"0.017\" led_medium=\"leds\" n_blocks=\"1\" n_edges=\"3\">\n"
                         "    </cparena>\n"
                         "    ...\n"
                         "  </arena>\n\n"
                         "Parameter description coming soon.\n\n",
                         "Under development"
                         );


   /****************************************/
   /****************************************/


   REGISTER_STANDARD_SPACE_OPERATIONS_ON_COMPOSABLE(CArenaEntity);

   /****************************************/
   /****************************************/

}
