
// Protocol132.h

// Interfaces to the cProtocol132 class representing the release 1.3.2 protocol (#39)





#pragma once

#include "Protocol125.h"
#include "cryptopp/modes.h"
#include "cryptopp/aes.h"





class cProtocol132 :
	public cProtocol125
{
	typedef cProtocol125 super;
public:

	cProtocol132(cClientHandle * a_Client);
	virtual ~cProtocol132();

	/// Called when client sends some data:
	virtual void DataReceived(const char * a_Data, int a_Size) override;
	
	// Sending commands (alphabetically sorted):
	virtual void SendBlockAction         (int a_BlockX, int a_BlockY, int a_BlockZ, char a_Byte1, char a_Byte2, BLOCKTYPE a_BlockType) override;
	virtual void SendBlockBreakAnim      (int a_EntityID, int a_BlockX, int a_BlockY, int a_BlockZ, char a_Stage) override;
	virtual void SendBlockChange         (int a_BlockX, int a_BlockY, int a_BlockZ, BLOCKTYPE a_BlockType, NIBBLETYPE a_BlockMeta) override;
	virtual void SendChunkData           (int a_ChunkX, int a_ChunkZ, cChunkDataSerializer & a_Serializer) override;
	virtual void SendCollectPickup       (const cPickup & a_Pickup, const cPlayer & a_Player) override;
	virtual void SendDestroyEntity       (const cEntity & a_Entity) override;
	virtual void SendEntityEquipment     (const cEntity & a_Entity, short a_SlotNum, const cItem & a_Item) override;
	virtual void SendLogin               (const cPlayer & a_Player, const cWorld & a_World) override;
	virtual void SendPlayerSpawn         (const cPlayer & a_Player) override;
	virtual void SendSoundEffect         (const AString & a_SoundName, int a_SrcX, int a_SrcY, int a_SrcZ, float a_Volume, float a_Pitch) override;  // a_Src coords are Block * 8
	virtual void SendSoundParticleEffect (int a_EffectID, int a_SrcX, int a_SrcY, int a_SrcZ, int a_Data) override;
	virtual void SendSpawnMob            (const cMonster & a_Mob) override;
	virtual void SendTabCompletionResults(const AStringVector & a_Results) override;
	virtual void SendUnloadChunk         (int a_ChunkX, int a_ChunkZ) override;
	virtual void SendWholeInventory      (const cWindow & a_Window) override;

	virtual AString GetAuthServerID(void) override;

	/// Handling of the additional packets:
	virtual int ParsePacket(unsigned char a_PacketType) override;
	
	// Modified packets:
	virtual int ParseBlockPlace     (void) override;
	virtual int ParseHandshake      (void) override;
	virtual int ParseLogin          (void) override;
	virtual int ParsePlayerAbilities(void) override;
	
	// New packets:
	virtual int ParseClientStatuses       (void);
	virtual int ParseEncryptionKeyResponse(void);
	virtual int ParseLocaleViewDistance   (void);
	virtual int ParseTabCompletion        (void);
	
protected:
	bool m_IsEncrypted;
	CryptoPP::CFB_Mode<CryptoPP::AES>::Decryption m_Decryptor;
	CryptoPP::CFB_Mode<CryptoPP::AES>::Encryption m_Encryptor;
	AString m_DataToSend;
	
	/// The ServerID used for session authentication; set in StartEncryption(), used in GetAuthServerID()
	AString m_AuthServerID;
	
	/// The server's public key, as used by SendEncryptionKeyRequest() and StartEncryption()
	AString m_ServerPublicKey;
	
	virtual void SendData(const char * a_Data, int a_Size) override;
	
	// DEBUG:
	virtual void Flush(void) override;
	
	// Items in slots are sent differently
	virtual void WriteItem(const cItem & a_Item) override;
	virtual int  ParseItem(cItem & a_Item) override;
	
	/// Parses the metadata that may come with the item.
	int ParseItemMetadata(cItem & a_Item, const AString & a_Metadata);

	virtual void SendCompass(const cWorld & a_World);
	virtual void SendEncryptionKeyRequest(void);
	
	/// Decrypts the key and nonce, checks nonce, starts the symmetric encryption
	void HandleEncryptionKeyResponse(const AString & a_EncKey, const AString & a_EncNonce);
	
	/// Starts the symmetric encryption with the specified key; also sets m_AuthServerID
	void StartEncryption(const byte * a_Key);
} ;




