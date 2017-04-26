#include "atprc522thread.h"

atpRc522Thread::atpRc522Thread(MFRC522 *rc522, QObject *parent) : QThread(parent){
    mfrc522 = rc522;
}

void atpRc522Thread::run(){
    while (true) {
        loop();
    }

}

void atpRc522Thread::loop(){
    continutCard.clear();
    qRegisterMetaType< QMap<byte, QString> >( "QMap<byte, QString>" );
    // Look for new cards
    if ( ! mfrc522->PICC_IsNewCardPresent())
        return;

    // Select one of the cards
    if ( ! mfrc522->PICC_ReadCardSerial())
        return;

    // Show some details of the PICC (that is: the tag/card)
    QByteArray* img = new QByteArray(reinterpret_cast<const char*>(mfrc522->uid.uidByte), mfrc522->uid.size);
    QString cardString = QString(img->toHex());

    qDebug() << "Card UID: " << cardString;
    continutCard[100] = cardString;

    //the picc type of the card
    byte piccType = mfrc522->PICC_GetType(mfrc522->uid.sak);

    qDebug() <<"PICC type: " << mfrc522->PICC_GetTypeName(piccType);

// Try the known default keys
    MFRC522::MIFARE_Key key;
    for (byte k = 0; k < NR_KNOWN_KEYS; k++) {
        // Copy the known key into the MIFARE_Key structure
        for (byte i = 0; i < MFRC522::MF_KEY_SIZE; i++) {
            key.keyByte[i] = knownKeys[k][i];
        }
        // Try the key
        if (try_key(&key)) {
            qDebug() << "Operatiune cu success";
            emit cardReadDetails(continutCard);
            // Found and reported on the key and block,
            // no need to try other keys for this PICC
            break;
        }
    }
}

/*
 * Read data from card
 */
QMap<byte, QString> atpRc522Thread::readAllCard(MFRC522::MIFARE_Key *key){

    QVector<byte> blockNos;
    blockNos.append(1);
    blockNos.append(2);
    blockNos.append(4);
    blockNos.append(5);
    blockNos.append(6);

//    for (int i = 0; i < 10; ++i) { // max blox = 64

    foreach (block, blockNos) {

        status = mfrc522->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522->uid));
        if (status != MFRC522::STATUS_OK) {
            qDebug() << status;
            return continutCard;
        }

        // Read block
        byte byteCount = sizeof(buffer);
        status = mfrc522->MIFARE_Read(block, buffer, &byteCount);
        if (status != MFRC522::STATUS_OK) {
            qDebug() << "MIFARE_Read() failed: " << mfrc522->GetStatusCodeName(status);
        } else {
            // Successful read
            qDebug() << " KEY: " << key->keyByte;
            QByteArray* img1 = new QByteArray(reinterpret_cast<const char*>((*key).keyByte), MFRC522::MF_KEY_SIZE);
            qDebug() << "IMG1" << img1;
            QString cardString1 = QString(img1->toHex());
            qDebug() << "Success with key: " << cardString1;
            // Dump block data
            qDebug() << buffer;
            QByteArray* img2 = new QByteArray(reinterpret_cast<const char*>(buffer), 16);
            QString cardString2 = QString(QByteArray::fromHex(img2->toHex())).trimmed();
            qDebug() << "Block " << block << ": "<< cardString2;
            continutCard[block] = cardString2;
        }
//        block++;
    }
    return continutCard;
}

/*
 *Write data on card
 */
bool atpRc522Thread::WriteCard(QMap<int, byte *> ceScriu, MFRC522::MIFARE_Key *key){
    qDebug() << "inainte de loop";
    foreach(int blockKey, ceScriu.keys()) {
        qDebug() << "in loop";
        byte *dataW = ceScriu.value(blockKey);
        block = blockKey;
        status = mfrc522->PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, key, &(mfrc522->uid)); //A
        if (status != MFRC522::STATUS_OK) {
          qDebug() << "PCD_Authenticate() failed: " << mfrc522->GetStatusCodeName(status);
           return false;
        }
   // Write block
        status = mfrc522->MIFARE_Write(block, dataW, 16);
        if (status != MFRC522::STATUS_OK) {
            qDebug() << "MIFARE_Write() failed: " << mfrc522->GetStatusCodeName(status);
            return false;
        } else qDebug() << "MIFARE_Write() success: block 1";
    };
    return true;
}

/*
 * Try using the PICC (the tag/card) with the given key to access block 0.
 * On success, it will show the key details, and dump the block data on Serial.
 *
 * @return true when the given key worked, false otherwise.
 */
bool atpRc522Thread::try_key(MFRC522::MIFARE_Key *key){

    qDebug() << readAllCard(key);
    QMap<int, byte *> myTestMap;
    byte nume[] = {
        0x41, 0x6e, 0x64, 0x72, //  1,  2,   3,  4,
        0x65, 0x69, 0x20, 0x20, //  5,  6,   7,  8,
        0x20, 0x20, 0x20, 0x20, //  9, 10, 255, 12,
        0x20, 0x20, 0x20, 0x20  // 13, 14,  15, 16
    };
    byte prenume[] = {
        0x41, 0x72, 0x73, 0x69, //  1,  2,   3,  4,
        0x6e, 0x74, 0x65, 0x20, //  5,  6,   7,  8,
        0x20, 0x20, 0x20, 0x20, //  9, 10, 255, 12,
        0x20, 0x20, 0x20, 0x20  // 13, 14,  15, 16
    };
    byte dob[] = {
        0x30, 0x34, 0x31, 0x31, //  1,  2,   3,  4,
        0x38, 0x34, 0x20, 0x20, //  5,  6,   7,  8,
        0x20, 0x20, 0x20, 0x20, //  9, 10, 255, 12,
        0x20, 0x20, 0x20, 0x20  // 13, 14,  15, 16
    };
    byte tel[] = {
        0x30, 0x37, 0x38, 0x39, //  1,  2,   3,  4,
        0x32, 0x39, 0x32, 0x33, //  5,  6,   7,  8,
        0x30, 0x32, 0x37, 0x20, //  9, 10, 255, 12,
        0x20, 0x20, 0x20, 0x20  // 13, 14,  15, 16
    };
    byte uid[] = {
        0x31, 0x20, 0x20, 0x20, //  1,  2,   3,  4,
        0x20, 0x20, 0x20, 0x20, //  5,  6,   7,  8,
        0x20, 0x20, 0x20, 0x20, //  9, 10, 255, 12,
        0x20, 0x20, 0x20, 0x20  // 13, 14,  15, 16
    };
    myTestMap[1]=uid;
    myTestMap[2]=dob;
    myTestMap[4]=nume;
    myTestMap[5]=prenume;
    myTestMap[6]=tel;
    qDebug() << "Inainte de write";
  // qDebug() << "SCRIE" << WriteCard(myTestMap, key);

    mfrc522->PICC_HaltA();       // Halt PICC
    mfrc522->PCD_StopCrypto1();  // Stop encryption on PCD
    if (continutCard.size()>4){
        return true;
    }
    return false;
}
