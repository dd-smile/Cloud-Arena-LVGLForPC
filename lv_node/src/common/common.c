/*
 * @Description:
 * @Author: mai
 * @LastEditors: mai
 * @Date: 2023-07-03 17:38:37
 * @LastEditTime: 2023-07-05 10:49:00
 * @FilePath: /new_home_smart/lv_node/src/common/common.c
 */
#include "ui_app.h"

static lv_style_t style_bg_mask;
lv_obj_t *Textarea_Pinyin;
char *inputVal;

lv_100ask_pinyin_dict_t lv_100ask_def_pinyin[] = {
    {"a", "啊阿呵吖"},
    {"ai", "埃挨哎唉哀皑蔼矮碍爱隘癌艾"},
    {"an", "按安暗岸俺案鞍氨胺厂广庵揞犴铵桉谙鹌埯黯"},
    {"ang", "昂肮盎仰"},
    {"ao", "凹敖熬翱袄傲奥懊澳"},
    {"ba", "芭捌叭吧笆八疤巴拔跋靶把坝霸罢爸扒耙"},
    {"bai", "白摆佰败拜柏百稗伯"},
    {"ban", "斑班搬扳颁板版扮拌伴瓣半办绊般"},
    {"bang", "邦帮梆榜绑棒镑傍谤膀磅蚌"},
    {"bao", "苞胞包褒雹保饱宝抱报豹鲍爆剥薄暴刨炮曝瀑堡"},
    {"bei", "杯碑悲卑北辈背贝钡倍狈备惫焙被"},
    {"ben", "奔苯本笨"},
    {"beng", "崩绷甭泵蹦迸蚌"},
    {"bi", "逼鼻比鄙笔彼碧蓖蔽毕毙毖币庇痹闭敝弊必壁避陛辟臂秘"},
    {"bian", "鞭边编贬变卞辨辩辫扁便遍"},
    {"biao", "标彪膘表"},
    {"bie", "鳖憋别瘪"},
    {"bin", "彬斌濒滨宾摈"},
    {"bing", "兵冰柄丙秉饼炳病并屏"},
    {"bo", "玻菠播拨钵博勃搏铂箔帛舶脖膊渤驳柏剥薄波泊卜般伯"},
    {"bu", "捕哺补埠布步簿部怖卜不埔堡"},
    {"ca", "擦"},
    {"cai", "猜裁材才财睬踩采彩菜蔡"},
    {"can", "餐残惭惨灿蚕参掺"},
    {"cang", "苍舱仓沧藏"},
    {"cao", "操糙槽曹草"},
    {"ce", "策册测厕侧"},
    {"cen", "参"},
    {"ceng", "层蹭曾"},
    {"cha", "插叉茶碴搽察岔诧茬查刹喳差"},
    {"chai", "柴豺拆差"},
    {"chan", "搀蝉馋谗缠铲产阐颤掺单"},
    {"chang", "昌猖场尝常偿肠厂畅唱倡长敞裳"},
    {"chao", "超抄钞潮巢吵炒朝嘲绰剿"},
    {"che", "扯撤掣彻澈车"},
    {"chen", "郴臣辰尘晨忱沉陈趁衬橙沈称秤"},
    {"cheng", "撑城成呈程惩诚承逞骋橙乘澄盛称秤"},
    {"chi", "痴持池迟弛驰耻齿侈赤翅斥炽吃匙尺"},
    {"chong", "充冲崇宠虫重"},
    {"chou", "抽酬畴踌稠愁筹仇绸瞅丑臭"},
    {"chu", "初出橱厨躇锄雏滁除楚础储矗搐触处畜"},
    {"chuai", "揣"},
    {"chuan", "川穿椽船喘串传"},
    {"chuang", "疮窗床闯创"},
    {"chui", "吹炊捶锤垂椎"},
    {"chun", "春椿醇唇淳纯蠢"},
    {"chuo", "戳绰"},
    {"ci", "疵茨磁雌辞慈瓷词此刺赐次伺兹差"},
    {"cong", "聪葱囱匆从丛"},
    {"cou", "凑"},
    {"cu", "粗醋簇促卒"},
    {"cuan", "蹿篡窜攒"},
    {"cui", "摧崔催脆瘁粹淬翠"},
    {"cun", "村存寸"},
    {"cuo", "磋搓措挫错撮"},
    {"da", "搭达答瘩打大"},
    {"dai", "歹傣戴带殆代贷袋待逮怠大呆"},
    {"dan", "耽担丹郸胆旦氮但惮淡诞蛋掸弹石单"},
    {"dang", "当挡党荡档"},
    {"dao", "刀捣蹈倒岛祷导到稻悼道盗"},
    {"de", "德得的地"},
    {"dei", ""},
    {"deng", "蹬灯登等瞪凳邓澄"},
    {"di", "低滴迪敌笛狄涤嫡抵蒂第帝弟递缔的堤翟底地提"},
    {"dian", "颠掂滇碘点典靛垫电甸店惦奠淀殿佃"},
    {"diao", "碉叼雕凋刁掉吊钓调"},
    {"die", "跌爹碟蝶迭谍叠"},
    {"ding", "盯叮钉顶鼎锭定订丁"},
    {"diu", "丢"},
    {"dong", "东冬董懂动栋冻洞侗恫"},
    {"dou", "兜抖斗陡豆逗痘都"},
    {"du", "督毒犊独堵睹赌杜镀肚渡妒都读度"},
    {"duan", "端短锻段断缎"},
    {"dui", "兑队对堆"},
    {"dun", "墩吨钝遁蹲敦顿囤盾"},
    {"duo", "掇哆多夺垛躲朵跺剁惰度舵堕"},
    {"e", "峨鹅俄额讹娥厄扼遏鄂饿阿蛾恶哦"},
    {"en", "恩"},
    {"er", "而耳尔饵洱二贰儿"},
    {"fa", "发罚筏伐乏阀法珐"},
    {"fan", "藩帆翻樊矾钒凡烦反返范贩犯饭泛番繁"},
    {"fang", "坊芳方肪房防妨仿访纺放"},
    {"fei", "菲非啡飞肥匪诽吠肺废沸费"},
    {"fen", "芬酚吩氛分纷坟焚汾粉奋份忿愤粪"},
    {"feng", "丰封枫蜂峰锋风疯烽逢缝讽奉凤冯"},
    {"fo", "佛"},
    {"fou", "否"},
    {"fu", "夫敷肤孵扶辐幅氟符伏俘服浮涪福袱弗甫抚辅俯釜斧腑府腐赴副覆赋复傅付阜父腹负富讣附妇缚咐佛拂脯"},
    {"ga", "噶嘎夹咖"},
    {"gai", "该改概钙溉盖芥"},
    {"gan", "干甘杆柑竿肝赶感秆敢赣乾"},
    {"gang", "冈刚钢缸肛纲岗港杠扛"},
    {"gao", "篙皋高膏羔糕搞稿镐告"},
    {"ge", "哥歌搁戈鸽疙割葛格阁隔铬个各胳革蛤咯"},
    {"gei", "给"},
    {"gen", "根跟"},
    {"geng", "耕更庚羹埂耿梗粳颈"},
    {"gong", "工攻功恭龚供躬公宫弓巩拱贡共汞"},
    {"gou", "钩勾沟苟狗垢构购够"},
    {"gu", "辜菇咕箍估沽孤姑古蛊骨股故顾固雇鼓谷贾"},
    {"gua", "刮瓜剐寡挂褂"},
    {"guai", "乖拐怪"},
    {"guan", "棺关官冠观管馆罐惯灌贯纶"},
    {"guang", "光逛广"},
    {"gui", "瑰规圭归闺轨鬼诡癸桂柜跪贵刽硅傀炔龟"},
    {"gun", "辊滚棍"},
    {"guo", "锅郭国果裹过涡"},
    {"ha", "蛤哈"},
    {"hai", "骸孩海氦亥害骇还咳"},
    {"han", "酣憨邯韩含涵寒函喊罕翰撼捍旱憾悍焊汗汉"},
    {"hang", "杭航夯吭巷行"},
    {"hao", "壕嚎豪毫郝好耗号浩镐貉"},
    {"he", "喝荷菏禾何盒阂河赫褐鹤贺核合涸吓呵貉和"},
    {"hei", "黑嘿"},
    {"hen", "痕很狠恨"},
    {"heng", "亨横衡恒哼行"},
    {"hong", "轰哄烘虹鸿洪宏弘红"},
    {"hou", "喉侯猴吼厚候后"},
    {"hu", "呼乎忽瑚壶葫胡蝴狐糊湖弧虎护互沪户唬和"},
    {"hua", "花华猾画化话哗滑划"},
    {"huai", "槐怀淮徊坏"},
    {"huan", "欢环桓缓换患唤痪豢焕涣宦幻还"},
    {"huang", "荒慌黄磺蝗簧皇凰惶煌晃幌恍谎"},
    {"hui", "灰挥辉徽恢蛔回毁悔慧卉惠晦贿秽烩汇讳诲绘会"},
    {"hun", "昏婚魂浑混荤"},
    {"huo", "活伙火获或惑霍货祸豁和"},
    {"i", ""},
    {"ji", "击圾基机畸积箕肌饥迹激讥鸡姬绩吉极棘辑籍集及急疾汲即嫉级挤几脊己蓟技冀季伎剂悸济寄寂计记既忌际妓继纪给稽缉祭藉期奇齐系"},
    {"jia", "家嘉枷佳加荚颊甲钾假稼架驾嫁夹贾价搅茄缴"},
    {"jian", "歼监坚尖笺间煎兼肩艰奸缄茧检柬碱拣捡简俭剪减荐鉴践贱键箭件健舰剑饯渐溅涧建槛见浅"},
    {"jiang", "僵姜浆江疆蒋桨奖讲匠酱将降强"},
    {"jiao", "椒礁焦胶交郊浇骄娇脚教轿较叫窖蕉嚼搅铰狡饺绞酵觉校矫侥角缴剿"},
    {"jie", "揭接皆秸街阶截劫节杰捷睫竭洁结姐戒界借介疥诫届桔解藉芥"},
    {"jin", "巾筋斤金今津襟紧锦仅谨进靳晋禁近烬浸尽劲"},
    {"jing", "荆兢茎睛晶鲸京惊精经井警静境敬镜径痉靖竟竞净劲粳景颈"},
    {"jiong", "炯窘"},
    {"jiu", "揪究纠玖韭久灸九酒厩救旧臼舅咎就疚"},
    {"ju", "鞠拘狙疽驹菊局矩举沮聚拒据巨具距踞锯俱惧炬剧车桔居咀句蛆足"},
    {"juan", "捐鹃娟倦眷绢卷圈"},
    {"jue", "撅攫抉掘倔爵决诀绝嚼觉角"},
    {"jun", "菌钧军君峻俊竣郡骏均浚"},
    {"ka", "喀咖卡咯"},
    {"kai", "开揩凯慨楷"},
    {"kan", "刊堪勘坎砍看槛嵌"},
    {"kang", "康慷糠抗亢炕扛"},
    {"kao", "考拷烤靠"},
    {"ke", "坷苛柯棵磕颗科可渴克刻客课壳呵咳"},
    {"ken", "肯啃垦恳"},
    {"keng", "坑吭"},
    {"kong", "空恐孔控"},
    {"kou", "抠口扣寇"},
    {"ku", "枯哭窟苦酷库裤"},
    {"kua", "夸垮挎跨胯"},
    {"kuai", "块筷侩快会"},
    {"kuan", "宽款"},
    {"kuang", "匡筐狂框矿眶旷况"},
    {"kui", "亏盔岿窥葵奎魁馈愧傀溃"},
    {"kun", "坤昆捆困"},
    {"kuo", "扩廓阔括"},
    {"la", "垃拉喇辣啦蜡腊落"},
    {"lai", "莱来赖"},
    {"lan", "婪栏拦篮阑兰澜谰揽览懒缆烂滥蓝"},
    {"lang", "琅榔狼廊郎朗浪"},
    {"lao", "捞劳牢老佬涝姥酪烙潦落"},
    {"le", "勒乐肋了"},
    {"lei", "雷镭蕾磊累儡垒擂类泪勒肋"},
    {"leng", "楞冷棱"},
    {"li", "厘梨犁黎篱狸离漓理李里鲤礼莉荔吏栗丽厉励砾历利例俐痢立粒沥隶力璃哩"},
    {"lian", "联莲连镰廉涟帘敛脸链恋炼练怜"},
    {"liang", "粮凉梁粱良两辆量晾亮谅俩"},
    {"liao", "撩聊僚疗燎寥辽撂镣廖料潦了"},
    {"lie", "列裂烈劣猎"},
    {"lin", "琳林磷霖临邻鳞淋凛赁吝拎"},
    {"ling", "玲菱零龄铃伶羚凌灵陵岭领另令棱怜"},
    {"liu", "溜琉榴硫馏留刘瘤流柳六陆"},
    {"long", "龙聋咙笼窿隆垄拢陇弄"},
    {"lou", "楼娄搂篓漏陋露"},
    {"lu", "芦卢颅庐炉掳卤虏鲁麓路赂鹿潞禄录戮吕六碌露陆绿"},
    {"lv", "驴铝侣旅履屡缕虑氯律滤绿率"},
    {"lve", "掠略"},
    {"luan", "峦挛孪滦卵乱"},
    {"lun", "抡轮伦仑沦论纶"},
    {"luo", "萝螺罗逻锣箩骡裸洛骆烙络落咯"},
    {"ma", "妈麻玛码蚂马骂嘛吗摩抹么"},
    {"mai", "买麦卖迈埋脉"},
    {"man", "瞒馒蛮满曼慢漫谩埋蔓"},
    {"mang", "茫盲氓忙莽芒"},
    {"mao", "猫茅锚毛矛铆卯茂帽貌贸冒"},
    {"me", "么"},
    {"mei", "玫枚梅酶霉煤眉媒镁每美昧寐妹媚没糜"},
    {"men", "门闷们"},
    {"meng", "萌蒙檬锰猛梦孟盟"},
    {"mi", "眯醚靡迷弥米觅蜜密幂糜谜泌秘"},
    {"mian", "棉眠绵冕免勉缅面娩"},
    {"miao", "苗描瞄藐秒渺庙妙"},
    {"mie", "蔑灭"},
    {"min", "民抿皿敏悯闽"},
    {"ming", "明螟鸣铭名命"},
    {"miu", "谬"},
    {"mo", "摸摹蘑膜磨魔末莫墨默沫漠寞陌脉没模摩抹"},
    {"mou", "谋某牟"},
    {"mu", "拇牡亩姆母墓暮幕募慕木目睦牧穆姥模牟"},
    {"na", "拿钠纳呐那娜哪"},
    {"nai", "氖乃奶耐奈哪"},
    {"nan", "南男难"},
    {"nang", "囊"},
    {"nao", "挠脑恼闹淖"},
    {"ne", "呢哪"},
    {"nei", "馁内那哪"},
    {"nen", "嫩"},
    {"neng", "能"},
    {"ni", "你妮霓倪泥尼拟匿腻逆溺呢"},
    {"nian", "蔫拈年碾撵捻念粘"},
    {"niang", "娘酿"},
    {"niao", "鸟尿"},
    {"nie", "捏聂孽啮镊镍涅"},
    {"nin", "您"},
    {"ning", "柠狞凝宁拧泞"},
    {"niu", "牛扭钮纽"},
    {"nong", "脓浓农弄"},
    {"nu", "奴怒努"},
    {"nv", "女"},
    {"nve", "虐疟"},
    {"nuan", "暖"},
    {"nuo", "挪懦糯诺娜"},
    {"o", "哦喔噢"},
    {"ou", "欧鸥殴藕呕偶沤区"},
    {"pa", "啪趴爬帕怕扒耙琶"},
    {"pai", "拍排牌徘湃派迫"},
    {"pan", "攀潘盘磐盼畔判叛番胖般"},
    {"pang", "乓庞耪膀磅旁胖"},
    {"pao", "抛咆袍跑泡刨炮"},
    {"pei", "呸胚培裴赔陪配佩沛坏"},
    {"pen", "喷盆"},
    {"peng", "砰抨烹澎彭蓬棚硼篷膨朋鹏捧碰"},
    {"pi", "坯砒霹批披劈琵毗啤脾疲皮痞僻屁譬辟否匹坏"},
    {"pian", "篇偏片骗扁便"},
    {"piao", "飘漂瓢票朴"},
    {"pie", "撇瞥"},
    {"pin", "拼频贫品聘"},
    {"ping", "乒坪萍平凭瓶评苹屏"},
    {"po", "坡泼颇婆破粕泊迫魄朴"},
    {"pou", "剖"},
    {"pu", "扑铺仆莆葡菩蒲圃普浦谱脯埔曝瀑堡朴"},
    {"qi", "欺戚妻七凄柒沏棋歧崎脐旗祈祁骑起岂乞企启器气迄弃汽讫稽缉期栖其奇畦齐砌泣漆契"},
    {"qia", "掐卡洽"},
    {"qian", "牵扦钎千迁签仟谦黔钱钳前潜遣谴堑欠歉铅乾浅嵌纤"},
    {"qiang", "枪呛腔羌墙蔷抢强"},
    {"qiao", "锹敲悄桥乔侨巧撬翘峭俏窍壳橇瞧鞘雀"},
    {"qie", "切窃砌茄且怯"},
    {"qin", "钦侵秦琴勤芹擒禽寝亲沁"},
    {"qing", "青轻氢倾卿清擎晴氰情顷请庆亲"},
    {"qiong", "琼穷"},
    {"qiu", "秋丘邱球求囚酋泅"},
    {"qu", "趋曲躯屈驱渠取娶龋去区蛆趣"},
    {"quan", "颧权醛泉全痊拳犬券劝卷圈"},
    {"que", "缺瘸却鹊榷确炔雀"},
    {"qun", "裙群"},
    {"ran", "然燃冉染"},
    {"rang", "瓤壤攘嚷让"},
    {"rao", "饶扰绕"},
    {"re", "惹热"},
    {"ren", "壬仁人忍韧任认刃妊纫"},
    {"reng", "扔仍"},
    {"ri", "日"},
    {"rong", "戎茸蓉荣融熔溶容绒冗"},
    {"rou", "揉柔肉"},
    {"ru", "茹儒孺如辱乳汝入褥蠕"},
    {"ruan", "软阮"},
    {"rui", "蕊瑞锐"},
    {"run", "闰润"},
    {"ruo", "弱若"},
    {"sa", "撒洒萨"},
    {"sai", "腮鳃赛塞"},
    {"san", "三叁伞散"},
    {"sang", "桑嗓丧"},
    {"sao", "搔骚扫嫂梢"},
    {"se", "瑟涩塞色"},
    {"sen", "森"},
    {"seng", "僧"},
    {"sha", "砂杀沙纱傻啥煞莎刹杉厦"},
    {"shai", "筛晒色"},
    {"shan", "珊苫山删煽衫闪陕擅赡膳善汕扇缮杉栅掺单"},
    {"shang", "墒伤商赏晌上尚裳汤"},
    {"shao", "捎稍烧芍勺韶少哨邵绍鞘梢召"},
    {"she", "奢赊舌舍赦摄慑涉社设蛇拾折射"},
    {"shei", "谁"},
    {"shen", "砷申呻伸身深绅神审婶肾慎渗沈甚参娠什"},
    {"sheng", "声生甥牲升绳剩胜圣乘省盛"},
    {"shi", "是师失狮施湿诗尸虱十时蚀实史矢使屎驶始式示士世柿事拭誓逝势是嗜噬适仕侍释饰市恃室视试匙石拾食识氏似嘘殖峙什"},
    {"shou", "收手首守寿授售受瘦兽熟"},
    {"shu", "蔬枢梳殊抒输叔舒淑疏书赎孰薯暑曙署蜀黍鼠述树束戍竖墅庶漱恕熟属术数"},
    {"shua", "刷耍"},
    {"shuai", "摔甩帅衰率"},
    {"shuan", "栓拴"},
    {"shuang", "霜双爽"},
    {"shui", "水睡税谁说"},
    {"shun", "吮瞬顺舜"},
    {"shuo", "硕朔烁数说"},
    {"si", "斯撕嘶私司丝死肆寺嗣四饲巳食思伺似"},
    {"song", "松耸怂颂送宋讼诵"},
    {"sou", "搜擞嗽艘"},
    {"su", "苏酥俗素速粟僳塑溯诉肃宿缩"},
    {"suan", "酸蒜算"},
    {"sui", "虽隋随绥髓碎岁穗遂隧祟尿"},
    {"sun", "孙损笋"},
    {"suo", "蓑梭唆琐索锁所莎缩"},
    {"ta", "塌他它她獭挞蹋踏塔拓"},
    {"tai", "胎苔抬台泰酞太态汰"},
    {"tan", "坍摊贪瘫滩坛檀痰潭谭谈坦毯袒碳探叹炭弹"},
    {"tang", "塘搪堂棠膛唐糖躺淌趟烫敞汤倘"},
    {"tao", "掏涛滔绦萄桃逃淘讨套陶"},
    {"te", "特"},
    {"teng", "藤腾疼誊"},
    {"ti", "梯剔踢锑题蹄啼体替嚏惕涕剃屉提"},
    {"tian", "天添填田甜恬舔腆蚕"},
    {"tiao", "挑条迢眺跳调"},
    {"tie", "贴铁帖"},
    {"ting", "厅烃汀廷停亭庭挺艇听"},
    {"tong", "通桐酮瞳同铜彤童桶捅筒统痛侗恫"},
    {"tou", "偷投头透"},
    {"tu", "秃突图徒途涂屠土吐兔凸余"},
    {"tuan", "湍团"},
    {"tui", "推颓腿蜕退褪"},
    {"tun", "吞屯臀囤"},
    {"tuo", "拖托脱鸵陀驼椭妥唾驮拓"},
    {"u", ""},
    {"v", ""},
    {"wa", "挖哇蛙洼娃瓦袜"},
    {"wai", "歪外"},
    {"wan", "豌弯湾玩顽丸烷完碗挽晚惋婉腕蔓皖宛万"},
    {"wang", "汪王枉网往旺望忘妄亡"},
    {"wei", "威巍微危韦违桅围唯惟为潍维苇萎委伟伪纬未味畏胃喂魏位渭谓慰卫尾蔚尉"},
    {"wen", "瘟温蚊文闻纹吻稳紊问"},
    {"weng", "嗡翁瓮"},
    {"wo", "挝蜗窝我斡卧握沃涡"},
    {"wu", "巫呜钨乌污诬屋芜梧吾吴毋武五捂午舞伍侮坞戊雾晤物勿务悟误恶无"},
    {"xi", "昔熙析西硒矽晰嘻吸锡牺稀息希悉膝夕惜熄烯汐犀檄袭席习媳喜隙细栖溪铣洗系戏"},
    {"xia", "瞎匣霞辖暇峡侠狭下虾厦夏吓"},
    {"xian", "掀锨先仙鲜咸贤衔舷闲涎弦嫌显险现献县腺馅羡宪陷限线铣纤"},
    {"xiang", "相厢镶香箱襄湘乡翔祥详想响享项橡像向象降巷"},
    {"xiao", "萧硝霄哮销消宵晓小孝肖啸笑效削嚣淆校"},
    {"xie", "楔些歇鞋协携胁谐写械卸蟹懈泄泻谢屑解蝎挟邪斜血叶契"},
    {"xin", "薪芯锌欣辛新忻心衅信"},
    {"xing", "星腥猩惺兴刑型形邢醒幸杏性姓省行"},
    {"xiong", "兄凶胸匈汹雄熊"},
    {"xiu", "休修羞朽嗅锈秀袖绣臭宿"},
    {"xu", "墟需虚须徐许蓄酗叙旭序恤絮婿绪续戌嘘畜吁"},
    {"xuan", "轩喧宣悬旋玄选癣眩绚"},
    {"xue", "靴薛学穴雪削血"},
    {"xun", "勋熏循旬询驯巡殉汛训讯逊迅浚寻"},
    {"ya", "压押鸦鸭呀丫牙蚜衙涯雅哑亚讶芽崖轧"},
    {"yan", "焉阉淹盐严研蜒岩延言颜阎炎沿奄掩眼衍演艳堰燕厌砚雁唁彦焰宴谚验铅咽烟殷"},
    {"yang", "殃央鸯秧杨扬佯疡羊洋阳氧仰痒养样漾"},
    {"yao", "邀腰妖瑶摇尧遥窑谣姚咬舀药要耀约钥侥"},
    {"ye", "椰噎耶爷野冶也页业夜咽掖叶腋液拽曳"},
    {"yi", "一壹医揖铱依伊衣颐夷移仪胰疑沂宜姨彝椅蚁倚已乙矣以艺抑易邑亿役臆逸肄疫亦裔意毅忆义益溢诣议谊译异翼翌绎遗屹"},
    {"yin", "茵荫因音阴姻吟银淫寅饮尹引隐印殷"},
    {"ying", "英樱婴鹰应缨莹萤营荧蝇迎赢盈影颖硬映"},
    {"yo", "哟"},
    {"yong", "拥佣臃痈庸雍踊蛹咏泳永恿勇用涌"},
    {"you", "幽优悠忧尤由邮铀犹油游酉有友右佑釉诱又幼"},
    {"yu", "迂淤于盂榆虞愚舆逾鱼愉渝渔隅予娱雨与屿禹宇语羽玉域芋郁遇喻峪御愈欲狱誉浴寓裕预豫驭尉余俞吁育"},
    {"yuan", "鸳渊冤元垣袁原援辕园圆猿源缘远苑愿怨院员"},
    {"yue", "曰越跃岳粤月悦阅乐约钥"},
    {"yun", "耘云郧匀陨允运蕴酝晕韵孕均员"},
    {"za", "匝砸杂扎咱咋"},
    {"zai", "栽哉灾宰载再在仔"},
    {"zan", "暂赞攒咱"},
    {"zang", "赃脏葬藏"},
    {"zao", "遭糟藻枣早澡蚤躁噪造皂灶燥凿"},
    {"ze", "责则泽择侧咋"},
    {"zei", "贼"},
    {"zen", "怎"},
    {"zeng", "增憎赠曾综"},
    {"zha", "渣札铡闸眨榨乍炸诈查扎喳栅柞轧咋"},
    {"zhai", "斋债寨翟祭择摘宅窄侧"},
    {"zhan", "瞻毡詹沾盏斩辗崭展蘸栈占战站湛绽颤粘"},
    {"zhang", "樟章彰漳张掌涨杖丈帐账仗胀瘴障长"},
    {"zhao", "招昭找沼赵照罩兆肇朝召爪着"},
    {"zhe", "遮哲蛰辙者蔗浙折锗这着"},
    {"zhen", "珍斟真甄砧臻贞针侦枕疹诊震振镇阵帧"},
    {"zheng", "蒸挣睁征狰争怔整拯正政症郑证"},
    {"zhi", "芝支蜘知肢脂汁之织职直植执值侄址指止趾只旨纸志挚掷至致置帜制智秩稚质炙痔滞治窒识枝吱殖峙"},
    {"zhong", "中盅忠钟衷终肿仲众种重"},
    {"zhou", "舟周州洲诌轴肘帚咒皱宙昼骤粥"},
    {"zhu", "珠株蛛朱猪诸诛逐竹烛煮拄瞩嘱主柱助蛀贮铸筑住注祝驻属著"},
    {"zhua", "抓爪"},
    {"zhuai", "拽"},
    {"zhuan", "专砖撰篆传转赚"},
    {"zhuang", "桩庄装妆壮状幢撞"},
    {"zhui", "锥追赘坠缀椎"},
    {"zhun", "谆准"},
    {"zhuo", "捉拙卓桌茁酌啄灼浊琢缴着"},
    {"zi", "咨资姿滋淄孜紫籽滓子自渍字吱兹仔"},
    {"zong", "鬃棕踪宗总纵综"},
    {"zou", "邹走奏揍"},
    {"zu", "租族祖诅阻组足卒"},
    {"zuan", "钻纂"},
    {"zui", "嘴醉最罪"},
    {"zun", "尊遵"},
    {"zuo", "做作坐昨左佐座撮琢柞"},
    {NULL, NULL}};

char *USRNET_MQTT_MSH[] = {
    MQTT_MSG_128MT_TEMPLATE("movable_stand_open", "1"),//0
    MQTT_MSG_128MT_TEMPLATE("movable_stand_stop", "1"),//1
    MQTT_MSG_128MT_TEMPLATE("movable_stand_stop", "0"),//2
    MQTT_MSG_128MT_TEMPLATE("movable_stand_shut", "1"),//3
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_down_1", "1"),//4
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_up_1", "1"),//5
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_down_2", "1"),//6
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_up_2", "1"),//7
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_down_3", "1"),//8
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_up_3", "1"),//9
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_down_4", "1"),//10
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_up_4", "1"),//11
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_stop_1", "1"),//12
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_stop_1", "0"),//13
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_stop_2", "1"),//14
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_stop_2", "0"),//15
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_stop_3", "1"),//16
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_stop_3", "0"),//17
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_stop_4", "1"),//18
    MQTT_MSG_128MT_TEMPLATE("Hanging_ball_rack_stop_4", "0"),//19
    MQTT_MSG_128MT_TEMPLATE("divider_down_1", "1"),//20
    MQTT_MSG_128MT_TEMPLATE("divider_up_1", "1"),//21
    MQTT_MSG_128MT_TEMPLATE("divider_down_2", "1"),//22
    MQTT_MSG_128MT_TEMPLATE("divider_up_2", "1"),//23
    MQTT_MSG_128MT_TEMPLATE("divider_stop_1", "1"),//24
    MQTT_MSG_128MT_TEMPLATE("divider_stop_1", "0"),//25
    MQTT_MSG_128MT_TEMPLATE("divider_stop_2", "1"),//26
    MQTT_MSG_128MT_TEMPLATE("divider_stop_2", "0"),//27
    MQTT_MSG_128MT_TEMPLATE("Wall_hanging_ball_rack_open_1", "1"),//28
    MQTT_MSG_128MT_TEMPLATE("Wall_hanging_ball_rack_shut_1", "1"),//29
    MQTT_MSG_128MT_TEMPLATE("Wall_hanging_ball_rack_open_2", "1"),//30
    MQTT_MSG_128MT_TEMPLATE("Wall_hanging_ball_rack_shut_2", "1"),//31
    MQTT_MSG_128MT_TEMPLATE("Wall_hanging_ball_rack_stop_1", "1"),//32
    MQTT_MSG_128MT_TEMPLATE("Wall_hanging_ball_rack_stop_1", "0"),//33
    MQTT_MSG_128MT_TEMPLATE("Wall_hanging_ball_rack_stop_2", "1"),//34
    MQTT_MSG_128MT_TEMPLATE("Wall_hanging_ball_rack_stop_2", "0"),//35
    MQTT_MSG_128MT_TEMPLATE("Floor_folding_seat_up_1", "1"),//36
    MQTT_MSG_128MT_TEMPLATE("Floor_folding_seat_shut_1", "1"),//37
    MQTT_MSG_128MT_TEMPLATE("Floor_folding_seat_up_2", "1"),//38
    MQTT_MSG_128MT_TEMPLATE("Floor_folding_seat_shut_2", "1"),//39
    MQTT_MSG_128MT_TEMPLATE("Floor_folding_sea_stop_1", "1"),//40
    MQTT_MSG_128MT_TEMPLATE("Floor_folding_sea_stop_1", "0"),//41
    MQTT_MSG_128MT_TEMPLATE("Floor_folding_sea_stop_2", "1"),//42
    MQTT_MSG_128MT_TEMPLATE("Floor_folding_sea_stop_2", "0"),//43

    MQTT_MSG_48MT_TEMPLATE("Stage_mode", "1"),//44
    MQTT_MSG_48MT_TEMPLATE("Chorus_mode", "1"),//45
    MQTT_MSG_48MT_TEMPLATE("Stage_closed_operation", "1"),//46
    MQTT_MSG_48MT_TEMPLATE("Stage_reset_opertion", "1"),//47
    MQTT_MSG_48MT_TEMPLATE("Stage_reset_opertion", "0"),//48
    MQTT_MSG_48MT_TEMPLATE("Stage_stop_operation", "1"),//49
    MQTT_MSG_48MT_TEMPLATE("Stage_stop_operation", "0"),//50

    MQTT_MSG_128MT_TEMPLATE("Revolving_stage_rise", "1"),//51
    MQTT_MSG_128MT_TEMPLATE("Revolving_stage_rise", "0"),//52
    MQTT_MSG_128MT_TEMPLATE("Rotary_stage_stop", "1"),//53
    MQTT_MSG_128MT_TEMPLATE("Rotary_stage_stop", "0"),//54
    MQTT_MSG_128MT_TEMPLATE("Rotary_stage_descent", "1"),//55
    MQTT_MSG_128MT_TEMPLATE("Rotary_stage_descent", "0"),//56
    MQTT_MSG_128MT_TEMPLATE("Rotary_stage_turning", "1"),//57
    MQTT_MSG_128MT_TEMPLATE("Rotary_stage_turning", "0"),//58
    MQTT_MSG_128MT_TEMPLATE("Rotary_stage_reversal", "1"),//59
    MQTT_MSG_128MT_TEMPLATE("Rotary_stage_reversal", "0"),//60
    MQTT_MSG_128MT_TEMPLATE("Master_scram", "1"),//61
    MQTT_MSG_128MT_TEMPLATE("Manual_mode", "1"),//62
    MQTT_MSG_128MT_TEMPLATE("Total_reduction", "1"),//63
    MQTT_MSG_128MT_TEMPLATE("Total_reduction", "0"),//64

};

static void lv_c_style_set_style_mask(lv_obj_t *obj)
{
    lv_style_init(&style_bg_mask); // 初始化遮蔽罩风格
    lv_style_set_bg_color(&style_bg_mask, lv_color_hex(0x11141A));
    lv_style_set_bg_opa(&style_bg_mask, 200);
    lv_obj_add_style(obj, &style_bg_mask, LV_PART_MAIN);
}

/**
 * 创建一个图片
 * @param parent        指向父对象的指针
 * @param src           要显示的图像数据
 * @param x             x轴的偏移量
 * @param y             y轴的偏移量
 * @return              
 * */
lv_obj_t *image_create(lv_obj_t *parent, const void *src, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *img = lv_img_create(parent);
    lv_img_set_src(img, src);
    lv_obj_align(img, LV_ALIGN_CENTER, x, y);
    return img;
}

/*返回上一级事件*/
void lv_back_event(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);

    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_del(obj);
    }
}
lv_obj_t *card_create_16_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_16, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 0, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}
lv_obj_t *create_16_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_16, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 2, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}

lv_obj_t *card_create_12_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &fout_12_text, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 0, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}
lv_obj_t *create_12_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 0, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}
// 绿色字体
lv_obj_t *create_12_Green_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0x43cf7c), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 0, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}

// 红色字体
lv_obj_t *create_12_Red_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xd43030), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 0, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}

// 蓝色字体
lv_obj_t *create_12_Bule_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0x00bfff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_12, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 0, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}

lv_obj_t *card_create_14_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &fout_14_text, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 2, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}

lv_obj_t *card_create_36_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &fout_36_text, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 2, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}

lv_obj_t *card_create_72_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &fout_72_text, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 2, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}

lv_obj_t *card_create_100_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &fout_100_text, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 2, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}

lv_obj_t *card_create_24_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &fout_24_text, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 2, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}

lv_obj_t *card_create_20_text(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *label = lv_label_create(parent);
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_set_style_text_letter_space(label, 2, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_CENTER, x, y);
    return label;
}

// 创建遮罩层
lv_obj_t *lv_c_create_mask_box(lv_obj_t *parent)
{
    lv_obj_t *gray_layer = lv_obj_create(parent); // 主页面灰色遮罩层
    lv_obj_remove_style_all(gray_layer);
    lv_obj_set_pos(gray_layer, 0, 0);
    lv_obj_set_size(gray_layer, lv_obj_get_width(parent), lv_obj_get_height(parent));
    lv_c_style_set_style_mask(gray_layer);
    lv_obj_add_event_cb(gray_layer, lv_back_event, LV_EVENT_CLICKED, NULL); // 添加返回事件
    return gray_layer;
}

// 创建模式遮罩层
lv_obj_t *lv_mode_create_mask_box(lv_obj_t *parent)
{
    lv_obj_t *gray_layer = lv_obj_create(parent); // 主页面灰色遮罩层
    lv_obj_remove_style_all(gray_layer);
    lv_obj_set_pos(gray_layer, 0, 0);
    lv_obj_set_size(gray_layer, lv_obj_get_width(parent), lv_obj_get_height(parent));
    lv_c_style_set_style_mask(gray_layer);
    // lv_obj_add_event_cb(gray_layer, lv_back_event, LV_EVENT_CLICKED, NULL); // 添加返回事件
    return gray_layer;
}

/*-----------------------------------------------创建键盘---------------------------------------------------------*/

/**
 * 返回上一级事件
 * @param e             指向事件描述符的指针     
 * */
void lv_back(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_del(obj->parent);
    }
}

/**
 * 创建按钮
 * @param parent             指向父对象的指针  
 * @param on_back            按钮类型，ｔｒｕｅ是返回按钮，ｆａｌｓｅ不是返回按钮
 * @param confirm_text       按钮文本
 * @param x_ofs              x轴的偏移量
 * @param y_ofs              y轴的偏移量
 * @return                   返回已创建按钮的指针 
 * */
lv_obj_t *btn_create_text(lv_obj_t *parent, bool on_back, const char *confirm_text, lv_coord_t x_ofs, lv_coord_t y_ofs)
{
    lv_obj_t *confirm_btn = lv_btn_create(parent);   //创建按钮
    lv_obj_set_size(confirm_btn, 85, 35);
    lv_obj_clear_flag(confirm_btn, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_align(confirm_btn, LV_ALIGN_TOP_LEFT, x_ofs, y_ofs);  
    lv_obj_set_style_radius(confirm_btn, 5, LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(confirm_btn, lv_color_make(26, 31, 46), LV_PART_MAIN);
    lv_obj_set_style_bg_color(confirm_btn, lv_color_make(33, 150, 243), LV_STATE_PRESSED);  //按下状态
    lv_obj_set_style_border_opa(confirm_btn, 80, LV_PART_MAIN);
    // lv_obj_set_style_bg_grad_dir(confirm_btn, LV_GRAD_DIR_VER, LV_STATE_DEFAULT);

    lv_obj_t *confirm_label = lv_label_create(confirm_btn);
    lv_label_set_text(confirm_label, confirm_text);    //设置按钮文本
    lv_obj_set_style_text_color(confirm_label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(confirm_label, &PuHuiTi_Regular_20, LV_STATE_DEFAULT);
    lv_obj_align(confirm_label, LV_ALIGN_CENTER, 0, 0);

    if (on_back) // 如果是返回按钮
    {
        lv_obj_add_event_cb(confirm_btn, lv_back, LV_EVENT_CLICKED, NULL); // 添加返回事件
    }
    return confirm_btn;
}

// 点击键盘页面的添加按钮事件
// static void add_event_btn(lv_event_t *e)    有死机bug
// {
//     lv_event_code_t code = lv_event_get_code(e);
//     lv_obj_t *obj = lv_event_get_target(e);

//     if (code == LV_EVENT_CLICKED)
//     {
//         // memset(add_device_list[0].device_name, 0, sizeof(add_device_list[0].device_name)); // 清空数组
//         inputVal = lv_textarea_get_text(Textarea_Pinyin);                                              // 获取添加设备名称输入框的值
//         printf("添加文本:%s\n", inputVal);
//         // if(Input_label == NULL)
//         // {
//         //     while (true)
//         //     {
//         //         printf("test \r\n");
//         //     }
//         // }
//         // strcat(add_device_list[0].device_name, inputVal);                                  // 拼接字符串
//         lv_label_set_text(Input_label, inputVal);                                             // 设置标签的值
//         lv_obj_del(obj->parent);
//     }
// }


/**
 * 创建一个点击设备名称的输入框弹出键盘的页面（带拼音的）
 * @param parent               指向父对象的指针
 * */
lv_obj_t *lv_keypage_create(lv_obj_t *parent, lv_event_cb_t event_cb)
{
    lv_obj_t *keypage = lv_obj_create(parent);
    lv_obj_set_size(keypage, 1024, 600);
    lv_obj_align(keypage, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(keypage, lv_color_make(255, 255, 255), LV_PART_MAIN);
    lv_obj_set_style_radius(keypage, 0, LV_PART_MAIN);
    lv_obj_set_style_border_width(keypage, 0, LV_PART_MAIN);
    lv_100ask_pinyin_ime_simple_test(keypage); // 创建键盘

    Textarea_Pinyin = lv_100ask_pinyin_ime_simple_test(keypage); // 创建键盘
    if (Textarea_Pinyin != NULL && Input_label != NULL)
    {
        char *text = lv_label_get_text(Input_label);
        lv_textarea_add_text(Textarea_Pinyin, text);
    }
    
    lv_obj_t *confirm_btn = btn_create_text(keypage, false, "Add", 520, 50); // 创建添加按钮
    lv_obj_add_event_cb(confirm_btn, event_cb, LV_EVENT_ALL, NULL);     // 添加事件
    btn_create_text(keypage, true, "Cancel", 620, 50);                       // 创建删除按钮

    return keypage;
}

// 创建设备名称的键盘（带拼音的）
lv_obj_t *lv_100ask_pinyin_ime_simple_test(lv_obj_t *parent)
{
    lv_obj_t *pinyin_ime = lv_100ask_pinyin_ime_create(parent);
    lv_obj_set_style_text_font(pinyin_ime, &fout_16_text, 0);
    lv_100ask_pinyin_ime_set_dict(pinyin_ime, lv_100ask_def_pinyin);

    lv_obj_t *pinyin_kb = lv_100ask_pinyin_ime_get_kb(pinyin_ime);
    lv_obj_set_size(pinyin_kb, 1024, 300);
    lv_obj_align(pinyin_kb, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t *KeyTextarea = lv_textarea_create(parent);
    lv_obj_set_size(KeyTextarea, 504, 50);
    lv_obj_set_style_radius(KeyTextarea, 5, LV_PART_MAIN);
    lv_obj_set_style_border_color(KeyTextarea, lv_color_make(72, 80, 103), LV_PART_MAIN);
    lv_obj_set_style_border_width(KeyTextarea, 1, LV_PART_MAIN);
    lv_obj_set_style_text_font(KeyTextarea, &fout_16_text, 0);
    lv_obj_align(KeyTextarea, LV_ALIGN_TOP_LEFT, 10, 40);
    lv_keyboard_set_textarea(pinyin_kb, KeyTextarea);
    return KeyTextarea;
}

/*-----------------------------------------------创建弹出框---------------------------------------------------------*/
// 返回上一级
void lv_back_Mask(lv_event_t *e)
{
    lv_event_code_t code = lv_event_get_code(e);
    lv_obj_t *obj = lv_event_get_target(e);
    if (code == LV_EVENT_CLICKED)
    {
        lv_obj_del(obj->parent->parent);
    }
}

/**
 * 创建一个弹出框
 * @param parent                 指向父对象的指针
 * @param title                  标题文本，以'\0'结尾的字符串
 * @param confirm_text           确认按钮文本
 * @param back_text              返回按钮文本
 * @param text                   正文内容，要设置文本字体的
 * @param event_cb               要注册的事件
 * @param user_data              自定义数据,数据将在' event_cb '中可用
 * @return                       返回指向图标的指针       
 * */
lv_obj_t *create_popup(lv_obj_t *parent, const char *title, const char *confirm_text, const char *back_text, const char *text, lv_event_cb_t event_cb, void *user_data)
{
    lv_obj_t *mask = lv_c_create_mask_box(parent);     // 创建遮罩
    lv_obj_t *iamge = image_create(mask, &Mask, 0, 0); // 创建图标
    lv_obj_t *label = lv_label_create(iamge);          // 创建标题
    lv_label_set_text(label, title);
    lv_obj_set_style_text_color(label, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(label, &fout_14_text, LV_STATE_DEFAULT);
    lv_obj_align(label, LV_ALIGN_TOP_LEFT, 70, 15);

    lv_obj_t *confirm_btn = btn_create_text(iamge, false, confirm_text, 135, 255); // 创建按钮
    lv_obj_add_event_cb(confirm_btn, event_cb, LV_EVENT_CLICKED, user_data);       // 添加返回事件
    lv_obj_t *Back_btn = btn_create_text(iamge, false, back_text, 280, 255);       // 创建按钮
    lv_obj_add_event_cb(Back_btn, event_cb, LV_EVENT_CLICKED, user_data);          // 添加返回事件
    card_create_36_text(iamge, text, 0, -20);

    return iamge;
}

/*-----------------------------------------------创建设置页面---------------------------------------------------------*/

/**
 * 创建一个设置页面的卡片
 * @param parent        指向父对象的指针
 * @return              返回指向卡片的指针
 * */
lv_obj_t *Createcard(lv_obj_t *parent)
{
    lv_obj_t *card = lv_obj_create(parent);
    lv_obj_set_size(card, 275, 179);
    lv_obj_set_style_bg_color(card, lv_color_make(31, 38, 51), LV_STATE_DEFAULT);
    lv_obj_set_style_radius(card, 20, LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(card, 0, LV_STATE_DEFAULT);
    return card;
}

/**
 * 创建一个类似进度条的东西
 * @param parent           指向一个对象的指针，它将是新图像的父对象
 * @param x                进度条的x坐标
 * @param y                进度条的y坐标
 * @param min              进度条最小值
 * @param max              进度条最大值
 * @param value            进度条的颜色
 * @param num_value        进度条的数值
 * @param w                进度条宽度
 * @param h                进度条高度
 * @param status           保留位
 * 
*/
lv_obj_t *CreateSlider(lv_obj_t *parent, lv_coord_t x, lv_coord_t y, int32_t min, int32_t max, lv_color_t value, int32_t num_value, lv_coord_t w, lv_coord_t h, bool status)
{
    //
    lv_obj_t *slider = lv_slider_create(parent);
    lv_obj_set_size(slider, w, h);
    lv_obj_align(slider, LV_ALIGN_CENTER, x, y);
    lv_slider_set_range(slider, min, max);
    lv_slider_set_value(slider, num_value, LV_ANIM_OFF);
    lv_obj_set_style_bg_opa(slider, 0, LV_PART_KNOB);
    lv_obj_set_style_bg_color(slider, lv_color_hex(0x5a748c), LV_PART_MAIN);
    lv_obj_set_style_bg_color(slider, value, LV_PART_INDICATOR);
    lv_obj_set_style_border_width(slider, 0, LV_PART_MAIN);
    lv_obj_add_flag(slider, LV_OBJ_FLAG_ADV_HITTEST);

    return slider;
}

static void get_time(char *buffer, size_t buffer_size)
{
    time_t t = time(NULL);
    struct tm *tmp = localtime(&t);
    snprintf(buffer, buffer_size, "%04d-%02d-%02d   %02d:%02d", tmp->tm_year + 1900, tmp->tm_mon + 1, tmp->tm_mday, tmp->tm_hour, tmp->tm_min);
}

/*刷新时间*/
static void lv_get_time(lv_timer_t *timer)
{
    lv_obj_t *obj = timer->user_data;
    static char time_buff[100];
    get_time(time_buff, sizeof(time_buff));
    lv_label_set_text(obj, time_buff);
}

/**创建时间 参数一：父对象  参数二：字体大小**/
lv_obj_t *create_recv_time(lv_obj_t *parent, const lv_font_t *value, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *recv_time = lv_label_create(parent);
    static char time_buff[100];
    get_time(time_buff, sizeof(time_buff));
    lv_label_set_text(recv_time, time_buff);
    lv_obj_align(recv_time, LV_ALIGN_CENTER, x, y);
    lv_obj_set_style_text_color(recv_time, lv_color_hex(0xffffff), LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(recv_time, value, LV_STATE_DEFAULT);
    lv_timer_create(lv_get_time, 1000, recv_time);
    return recv_time;
}

/**
 * 创建点击对象(矩形)
 * @param obj         指向父对象的指针
 * @return            返回点击对象
 * */
lv_obj_t *CreateOBJclick(lv_obj_t *obj)
{
    lv_obj_t *Obj = lv_obj_create(obj);
    lv_obj_set_size(Obj, 200, 140);
    lv_obj_align(Obj, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(Obj, LV_OPA_0, 0);
    lv_obj_set_style_border_width(Obj, 0, LV_ALIGN_DEFAULT);

    return Obj;
}


// 创建一个弹出页面
lv_obj_t *create_popup_page(lv_obj_t *parent, const char *title, lv_coord_t x, lv_coord_t y)
{
    lv_obj_t *popup_page = lv_obj_create(parent);
    lv_obj_set_size(popup_page, 1024, 600);
    lv_obj_align(popup_page, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(popup_page, lv_color_hex(0x141a23), 0);
    lv_obj_set_style_border_width(popup_page, 0, LV_STATE_DEFAULT);

    lv_obj_t *Titleobj = lv_obj_create(parent);
    lv_obj_set_size(Titleobj, 1024, 64);
    lv_obj_align(Titleobj, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_style_bg_color(Titleobj, lv_color_hex(0x1f2633), 0);
    lv_obj_set_style_border_width(Titleobj, 0, LV_STATE_DEFAULT);
    lv_obj_set_style_radius(Titleobj, 0, LV_PART_MAIN);
    lv_obj_clear_flag(Titleobj, LV_OBJ_FLAG_SCROLLABLE); // 禁用滚动条
    card_create_24_text(Titleobj, title, x, y);
    create_page_back(Titleobj); // 返回上一级按钮

    return popup_page;
}

//打印内存情况
void lv_memory_print(void)
{
    lv_mem_monitor_t mem;
    lv_mem_monitor(&mem);
    printf("LV_MEM===> total:%d free:%d max_free :%d max_used :%d cnt %d/%d frg %d/%d \n", mem.total_size, mem.free_size,
    mem.free_biggest_size, mem.max_used, mem.free_cnt, mem.used_cnt, mem.used_pct,mem.frag_pct);
}

