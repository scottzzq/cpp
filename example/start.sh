ulimit -c unlimited
cp ./tikv_server ./store1/
cd ./store1/
rm -rf core*
rm -rf tikv_server.log
rm -rf nohup.out
nohup ./tikv_server ./tikv_server.conf &
cd ../

sleep 1

cp ./tikv_server ./store2/
cd ./store2/
rm -rf core*
rm -rf nohup.out
rm -rf tikv_server.log
nohup ./tikv_server ./tikv_server.conf &
cd ../

sleep 1

cp ./tikv_server ./store3/
cd ./store3/
rm -rf core*
rm -rf tikv_server.log
rm -rf nohup.out
nohup ./tikv_server ./tikv_server.conf &
cd ../

tailf store1/tikv_server.log
