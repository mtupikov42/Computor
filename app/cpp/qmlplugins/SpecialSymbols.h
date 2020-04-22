#pragma once

#include <QObject>

class SpecialSymbols final : public QObject {
    Q_OBJECT
	Q_PROPERTY(QChar Celsius MEMBER(Celsius) CONSTANT)
	Q_PROPERTY(QChar DegreeSign MEMBER(DegreeSign) CONSTANT)
	Q_PROPERTY(QChar faAlignLeft MEMBER(AlignLeft) CONSTANT)
	Q_PROPERTY(QChar faAngleDoubleDown MEMBER(AngleDoubleDown) CONSTANT)
	Q_PROPERTY(QChar faAngleDoubleLeft MEMBER(AngleDoubleLeft) CONSTANT)
	Q_PROPERTY(QChar faAngleDoubleRight MEMBER(AngleDoubleRight) CONSTANT)
	Q_PROPERTY(QChar faAngleDoubleUp MEMBER(AngleDoubleUp) CONSTANT)
	Q_PROPERTY(QChar faAngleDown MEMBER(AngleDown) CONSTANT)
	Q_PROPERTY(QChar faAngleLeft MEMBER(AngleLeft) CONSTANT)
	Q_PROPERTY(QChar faAngleRight MEMBER(AngleRight) CONSTANT)
	Q_PROPERTY(QChar faAngleUp MEMBER(AngleUp) CONSTANT)
	Q_PROPERTY(QChar faArrowCircleDown MEMBER(ArrowCircleDown) CONSTANT)
	Q_PROPERTY(QChar faArrowCircleLeft MEMBER(ArrowCircleLeft) CONSTANT)
	Q_PROPERTY(QChar faArrowCircleRight MEMBER(ArrowCircleRight) CONSTANT)
	Q_PROPERTY(QChar faArrowCircleUp MEMBER(ArrowCircleUp) CONSTANT)
	Q_PROPERTY(QChar faArrowDown MEMBER(ArrowDown) CONSTANT)
	Q_PROPERTY(QChar faArrowLeft MEMBER(ArrowLeft) CONSTANT)
	Q_PROPERTY(QChar faArrowRight MEMBER(ArrowRight) CONSTANT)
	Q_PROPERTY(QChar faArrowUp MEMBER(ArrowUp) CONSTANT)
	Q_PROPERTY(QChar faAscendingAlfabetical MEMBER(AscendingAlfabetical) CONSTANT)
	Q_PROPERTY(QChar faAscendingAmount MEMBER(AscendingAmount) CONSTANT)
	Q_PROPERTY(QChar faAsterisk MEMBER(Asterisk) CONSTANT)
	Q_PROPERTY(QChar faBell MEMBER(Bell) CONSTANT)
	Q_PROPERTY(QChar faCalendar MEMBER(Calendar) CONSTANT)
	Q_PROPERTY(QChar faCaretDown MEMBER(CaretDown) CONSTANT)
	Q_PROPERTY(QChar faCaretLeft MEMBER(CaretLeft) CONSTANT)
	Q_PROPERTY(QChar faCaretRight MEMBER(CaretRight) CONSTANT)
	Q_PROPERTY(QChar faCaretUp MEMBER(CaretUp) CONSTANT)
	Q_PROPERTY(QChar faCheckCircle MEMBER(CheckCircle) CONSTANT)
	Q_PROPERTY(QChar faChecked MEMBER(Checked) CONSTANT)
	Q_PROPERTY(QChar faCheckSquare MEMBER(CheckSquare) CONSTANT)
	Q_PROPERTY(QChar faChevronDown MEMBER(ChevronDown) CONSTANT)
	Q_PROPERTY(QChar faChevronLeft MEMBER(ChevronLeft) CONSTANT)
	Q_PROPERTY(QChar faChevronRight MEMBER(ChevronRight) CONSTANT)
	Q_PROPERTY(QChar faChevronUp MEMBER(ChevronUp) CONSTANT)
	Q_PROPERTY(QChar faCircle MEMBER(Circle) CONSTANT)
	Q_PROPERTY(QChar faClock MEMBER(Clock) CONSTANT)
	Q_PROPERTY(QChar faClone MEMBER(Clone) CONSTANT)
	Q_PROPERTY(QChar faCloudUpload MEMBER(CloudUploadAlt) CONSTANT)
	Q_PROPERTY(QChar faCollapse MEMBER(Collapse) CONSTANT)
	Q_PROPERTY(QChar faComment MEMBER(Comment) CONSTANT)
	Q_PROPERTY(QChar faCommentAlt MEMBER(CommentAlt) CONSTANT)
	Q_PROPERTY(QChar faCube MEMBER(Cube) CONSTANT)
	Q_PROPERTY(QChar faCubes MEMBER(Cubes) CONSTANT)
	Q_PROPERTY(QChar faDescendingAlfabetical MEMBER(DescendingAlfabetical) CONSTANT)
	Q_PROPERTY(QChar faDescendingAmount MEMBER(DescendingAmount) CONSTANT)
	Q_PROPERTY(QChar faDownload MEMBER(Download) CONSTANT)
	Q_PROPERTY(QChar faEllipsis MEMBER(Ellipsis) CONSTANT)
	Q_PROPERTY(QChar faExclamationCircle MEMBER(ExclamationCircle) CONSTANT)
	Q_PROPERTY(QChar faExclamationTriangle MEMBER(ExclamationTriangle) CONSTANT)
	Q_PROPERTY(QChar faExpand MEMBER(Expand) CONSTANT)
	Q_PROPERTY(QChar faFacebook MEMBER(Facebook) CONSTANT)
	Q_PROPERTY(QChar faFileAudio MEMBER(FileAudio) CONSTANT)
	Q_PROPERTY(QChar faFileExport MEMBER(FileExport) CONSTANT)
	Q_PROPERTY(QChar faFileImport MEMBER(FileImport) CONSTANT)
	Q_PROPERTY(QChar faFiles MEMBER(Files) CONSTANT)
	Q_PROPERTY(QChar faFlag MEMBER(Flag) CONSTANT)
	Q_PROPERTY(QChar faFloppy MEMBER(Floppy) CONSTANT)
	Q_PROPERTY(QChar faFolder MEMBER(Folder) CONSTANT)
	Q_PROPERTY(QChar faFolderOpen MEMBER(FolderOpen) CONSTANT)
	Q_PROPERTY(QChar faGamepad MEMBER(Gamepad) CONSTANT)
	Q_PROPERTY(QChar faGear MEMBER(Gear) CONSTANT)
	Q_PROPERTY(QChar faGooglePlus MEMBER(GooglePlus) CONSTANT)
	Q_PROPERTY(QChar faHealth MEMBER(Health) CONSTANT)
	Q_PROPERTY(QChar faImage MEMBER(Image) CONSTANT)
	Q_PROPERTY(QChar faInfinity MEMBER(Infinity) CONSTANT)
	Q_PROPERTY(QChar faInfo MEMBER(Info) CONSTANT)
	Q_PROPERTY(QChar faInfoCircle MEMBER(InfoCircle) CONSTANT)
	Q_PROPERTY(QChar faLightBulb MEMBER(LightBulb) CONSTANT)
	Q_PROPERTY(QChar faLink MEMBER(Link) CONSTANT)
	Q_PROPERTY(QChar faList MEMBER(List) CONSTANT)
	Q_PROPERTY(QChar faListNumbered MEMBER(ListNumbered) CONSTANT)
	Q_PROPERTY(QChar faLock MEMBER(Lock) CONSTANT)
	Q_PROPERTY(QChar faLogout MEMBER(Logout) CONSTANT)
	Q_PROPERTY(QChar faLongArrowAltDown MEMBER(LongArrowAltDown) CONSTANT)
	Q_PROPERTY(QChar faMenu MEMBER(Menu) CONSTANT)
	Q_PROPERTY(QChar faMinus MEMBER(Minus) CONSTANT)
	Q_PROPERTY(QChar faMinusSquare MEMBER(MinusSquare) CONSTANT)
	Q_PROPERTY(QChar faObjectGroup MEMBER(ObjectGroup) CONSTANT)
	Q_PROPERTY(QChar faObjectUngroup MEMBER(ObjectUngroup) CONSTANT)
	Q_PROPERTY(QChar faPauseCircle MEMBER(PauseCircle) CONSTANT)
	Q_PROPERTY(QChar faPlayCircle MEMBER(PlayCircle) CONSTANT)
	Q_PROPERTY(QChar faPlus MEMBER(Plus) CONSTANT)
	Q_PROPERTY(QChar faPlusSquare MEMBER(PlusSquare) CONSTANT)
	Q_PROPERTY(QChar faPower MEMBER(Power) CONSTANT)
	Q_PROPERTY(QChar faQuestionCircle MEMBER(QuestionCircle) CONSTANT)
	Q_PROPERTY(QChar faQuestionMark MEMBER(QuestionMark) CONSTANT)
	Q_PROPERTY(QChar faRandom MEMBER(Random) CONSTANT)
	Q_PROPERTY(QChar faRedo MEMBER(Redo) CONSTANT)
	Q_PROPERTY(QChar faRepeat MEMBER(Repeat) CONSTANT)
	Q_PROPERTY(QChar faReplyAll MEMBER(ReplyAll) CONSTANT)
	Q_PROPERTY(QChar faReset MEMBER(Reset) CONSTANT)
	Q_PROPERTY(QChar faRetweet MEMBER(Retweet) CONSTANT)
	Q_PROPERTY(QChar faRss MEMBER(Rss) CONSTANT)
	Q_PROPERTY(QChar faSearch MEMBER(Search) CONSTANT)
	Q_PROPERTY(QChar faSearchMinus MEMBER(SearchMinus) CONSTANT)
	Q_PROPERTY(QChar faSearchPlus MEMBER(SearchPlus) CONSTANT)
	Q_PROPERTY(QChar faSend MEMBER(Send) CONSTANT)
	Q_PROPERTY(QChar faShare MEMBER(Share) CONSTANT)
	Q_PROPERTY(QChar faShoppingCart MEMBER(ShoppingCart) CONSTANT)
	Q_PROPERTY(QChar faSiteMap MEMBER(SiteMap) CONSTANT)
	Q_PROPERTY(QChar faSliders MEMBER(Sliders) CONSTANT)
	Q_PROPERTY(QChar faSortAmountAsc MEMBER(SortAmountAsc) CONSTANT)
	Q_PROPERTY(QChar faSortBidirectional MEMBER(SortBidirectional) CONSTANT)
	Q_PROPERTY(QChar faSquare MEMBER(Square) CONSTANT)
	Q_PROPERTY(QChar faStar MEMBER(Star) CONSTANT)
	Q_PROPERTY(QChar faStopCircle MEMBER(StopCircle) CONSTANT)
	Q_PROPERTY(QChar faSync MEMBER(Sync) CONSTANT)
	Q_PROPERTY(QChar faTable MEMBER(Table) CONSTANT)
	Q_PROPERTY(QChar faTachometer MEMBER(Tachometer) CONSTANT)
	Q_PROPERTY(QChar faTags MEMBER(Tags) CONSTANT)
	Q_PROPERTY(QChar faTh MEMBER(Th) CONSTANT)
	Q_PROPERTY(QChar faThermometerFull MEMBER(ThermometerFull) CONSTANT)
	Q_PROPERTY(QChar faThList MEMBER(ThList) CONSTANT)
	Q_PROPERTY(QChar faThumbsUpClosed MEMBER(ThumbsUpClosed) CONSTANT)
	Q_PROPERTY(QChar faThumbsUpOpen MEMBER(ThumbsUpOpen) CONSTANT)
	Q_PROPERTY(QChar faTimes MEMBER(Times) CONSTANT)
	Q_PROPERTY(QChar faTimesCircle MEMBER(TimesCircle) CONSTANT)
	Q_PROPERTY(QChar faTrash MEMBER(TrashAlt) CONSTANT)
	Q_PROPERTY(QChar faUnchecked MEMBER(Unchecked) CONSTANT)
	Q_PROPERTY(QChar faUnlink MEMBER(Unlink) CONSTANT)
	Q_PROPERTY(QChar faUnlock MEMBER(Unlock) CONSTANT)
	Q_PROPERTY(QChar faUpload MEMBER(Upload) CONSTANT)
	Q_PROPERTY(QChar faUser MEMBER(User) CONSTANT)
	Q_PROPERTY(QChar faVolumeDown MEMBER(VolumeDown) CONSTANT)
	Q_PROPERTY(QChar faVolumeMute MEMBER(VolumeMute) CONSTANT)
	Q_PROPERTY(QChar faVolumeUp MEMBER(VolumeUp) CONSTANT)
	Q_PROPERTY(QChar faWeibo MEMBER(Weibo) CONSTANT)
	Q_PROPERTY(QChar faWind MEMBER(Wind) CONSTANT)
	Q_PROPERTY(QChar faWindowClose MEMBER(Times) CONSTANT)
	Q_PROPERTY(QChar faWindowMaximize MEMBER(WindowMaximize) CONSTANT)
	Q_PROPERTY(QChar faWindowMinimize MEMBER(WindowMinimize) CONSTANT)
	Q_PROPERTY(QChar faWindowRestore MEMBER(WindowRestore) CONSTANT)

public:
    static void RegisterType();

private:
	SpecialSymbols(QObject* parent = nullptr);

	const QChar AlignLeft;
	const QChar AngleDoubleDown;
	const QChar AngleDoubleLeft;
	const QChar AngleDoubleRight;
	const QChar AngleDoubleUp;
	const QChar AngleDown;
	const QChar AngleLeft;
	const QChar AngleRight;
	const QChar AngleUp;
	const QChar ArrowCircleDown;
	const QChar ArrowCircleLeft;
	const QChar ArrowCircleRight;
	const QChar ArrowCircleUp;
	const QChar ArrowDown;
	const QChar ArrowLeft;
	const QChar ArrowRight;
	const QChar ArrowUp;
	const QChar AscendingAlfabetical;
	const QChar AscendingAmount;
	const QChar Asterisk;
	const QChar Bell;
	const QChar Calendar;
	const QChar CaretDown;
	const QChar CaretLeft;
	const QChar CaretRight;
	const QChar CaretUp;
	const QChar Celsius;
	const QChar CheckCircle;
	const QChar Checked;
	const QChar CheckSquare;
	const QChar ChevronDown;
	const QChar ChevronLeft;
	const QChar ChevronRight;
	const QChar ChevronUp;
	const QChar Circle;
	const QChar Clock;
	const QChar Clone;
	const QChar CloudUploadAlt;
	const QChar Collapse;
	const QChar Comment;
	const QChar CommentAlt;
	const QChar Cube;
	const QChar Cubes;
	const QChar DegreeSign;
	const QChar DescendingAlfabetical;
	const QChar DescendingAmount;
	const QChar Download;
	const QChar Ellipsis;
	const QChar ExclamationCircle;
	const QChar ExclamationTriangle;
	const QChar Expand;
	const QChar Facebook;
	const QChar FileAudio;
	const QChar FileExport;
	const QChar FileImport;
	const QChar Files;
	const QChar Flag;
	const QChar Floppy;
	const QChar Folder;
	const QChar FolderOpen;
	const QChar Gamepad;
	const QChar Gear;
	const QChar GooglePlus;
	const QChar Health;
	const QChar Image;
	const QChar Infinity;
	const QChar Info;
	const QChar InfoCircle;
	const QChar LightBulb;
	const QChar Link;
	const QChar List;
	const QChar ListNumbered;
	const QChar Lock;
	const QChar Logout;
	const QChar LongArrowAltDown;
	const QChar Menu;
	const QChar Minus;
	const QChar MinusSquare;
	const QChar ObjectGroup;
	const QChar ObjectUngroup;
	const QChar PauseCircle;
	const QChar PlayCircle;
	const QChar Plus;
	const QChar PlusSquare;
	const QChar Power;
	const QChar QuestionCircle;
	const QChar QuestionMark;
	const QChar Random;
	const QChar Redo;
	const QChar Repeat;
	const QChar ReplyAll;
	const QChar Reset;
	const QChar Retweet;
	const QChar Rss;
	const QChar Search;
	const QChar SearchMinus;
	const QChar SearchPlus;
	const QChar Send;
	const QChar Share;
	const QChar ShoppingCart;
	const QChar SiteMap;
	const QChar Sliders;
	const QChar SortAmountAsc;
	const QChar SortBidirectional;
	const QChar Square;
	const QChar Star;
	const QChar StopCircle;
	const QChar Sync;
	const QChar Table;
	const QChar Tachometer;
	const QChar Tags;
	const QChar Th;
	const QChar ThermometerFull;
	const QChar ThList;
	const QChar ThumbsUpClosed;
	const QChar ThumbsUpOpen;
	const QChar Times;
	const QChar TimesCircle;
	const QChar TrashAlt;
	const QChar Unchecked;
	const QChar Unlink;
	const QChar Unlock;
	const QChar Upload;
	const QChar User;
	const QChar VolumeDown;
	const QChar VolumeMute;
	const QChar VolumeUp;
	const QChar Weibo;
	const QChar Wind;
	const QChar WindowMaximize;
	const QChar WindowMinimize;
	const QChar WindowRestore;
};
