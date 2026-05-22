```bash
docker build -t flavor-creator -f backend/Dockerfile backend
docker tag flavor-creator:latest ccr.ccs.tencentyun.com/tcb-100047820175-rwpe/ca-reflbqff_flcr2:v3
docker push ccr.ccs.tencentyun.com/tcb-100047820175-rwpe/ca-reflbqff_flcr2:v3
```

登陆微信云
```bash
docker login ccr.ccs.tencentyun.com --username=100047820175
```

