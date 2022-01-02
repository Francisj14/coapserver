import logging
import asyncio
import struct
import pymysql
import sys
from aiocoap import *
from datetime import date

logging.basicConfig(level=logging.INFO)
#ciclo while para pedir de forma continua los datos
today = date.today()
while True:
    
    async def main():

        protocol = await Context.create_client_context()
    #Establecemos los URI para cada uno de los sensores

        request = Message(code=GET, uri='coap://192.168.100.60/temperatura')
        request2 = Message(code=GET, uri='coap://192.168.100.60/humedad')
        request3 = Message(code=GET, uri='coap://192.168.100.60/distancia')

        #Petición al servidor de temperatura
        try:
            #Petición al servidor de temperatura
            response = await protocol.request(request).response
            #Petición al servidor de la humedas
            response1 = await protocol.request(request2).response
            #Petición al servidor de la distancia
            response2 = await protocol.request(request3).response
        except Exception as e:
            print('Failed to fetch resource:')
            print(e)
        else:
            #Recolección de los datos y transformación de bytes a tipo float
            #print('Result: %s\n%r'%(response.code, response.payload))
            t=response.payload;
            temp=t.decode('utf-8')
            temperatura=float(temp)
            print(temperatura)

            #print('Result: %s\n%r'%(response1.code, response1.payload))
            h=response1.payload;
            hum=h.decode('utf-8')
            humedad=float(hum)
            print(hum)

            #print('Result: %s\n%r'%(response2.code, response2.payload))
            d=response2.payload;
            dist=d.decode('utf-8')
            distancia=float(dist)
            #print(dist)
            #Conexión a la base datos
            Host = "localhost"  
            User = "root"       
            Password = "joel123"           
            Database = "sensores"
            conn  = pymysql.connect(host=Host, user=User, password=Password, database=Database)
            cur  = conn.cursor()
            #today = date.today()
            print(today)
            query = f"INSERT INTO tablasensores (temperatura,humedad,fecha) VALUES ('{temperatura}', '{humedad}', '{today}')"
    
            cur.execute(query)
            print(f"{cur.rowcount} fila insertada")
            conn.commit()
            conn.close()


#conexión a la base de datos

    if __name__ == "__main__":
        asyncio.get_event_loop().run_until_complete(main())
