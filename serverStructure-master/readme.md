注册:
请求端:
    request: {"type":1, "username":"XXX", "passwd":"XXX"}
响应端:
    response: {"type":1, "result":"success", "reason":"XXX"}

登录
请求端:
    request: {"type":2, "username":"XXX", "passwd":"XXX"}
响应端:
    response: {"type":2, "result":"success", "reason":"XXX"}


搜索歌曲:
请求端:
    request: {"type":SEARCH_MUSIC, "musicname": "我怀念的"}
响应端:
    response: {"type":SEARCH_MUSIC, "musicname": "我怀念的", "musiccontent": "XXXXX"}


下载音乐
请求端: 
    request: {"type" : DOWNLOAD, "musicname": "我怀念的"}
响应端:
    response: {"type" : DOWNLOAD, "musicname": "我怀念的", "musiccontent": "XXXXX"}